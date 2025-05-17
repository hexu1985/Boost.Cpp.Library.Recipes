#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>

#include <asio.hpp>

int calculate(int opnum, int opnds[], char oprator);

int main(int argc, char* argv[])
{
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	// The size of the queue containing the pending connection
	// requests.
	const int BACKLOG_SIZE = 5;

	// Step 1. Here we assume that the server application has
	// already obtained the protocol port number.
	unsigned short port_num = std::stoi(argv[1]);

	// Step 2. Creating a server endpoint.
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(),
		port_num);

	asio::io_context io;

    std::ifstream ifile("file_server.cpp", std::ios::in | std::ios::binary);
    if (!ifile) {
        std::cout << "open file_server.cpp failed!\n";
        return -1;
    }

	try {
		// Step 3. Instantiating and opening an acceptor socket.
		asio::ip::tcp::acceptor acceptor(io, ep.protocol());

        acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));

		// Step 4. Binding the acceptor socket to the 
		// server endpint.
		acceptor.bind(ep);

		// Step 5. Starting to listen to incoming connection
		// requests.
		acceptor.listen(BACKLOG_SIZE);

        // Step 6. Creating an active socket.
        asio::ip::tcp::socket sock(io);

        // Step 7. Processing the next connection request and 
        // connecting the active socket to the client.
        acceptor.accept(sock);

        // At this point 'sock' socket is connected to 
        //the client application and can be used to send data to
        // or receive data from it.
        const int BUF_SIZE = 30;
        char buf[BUF_SIZE];
        int read_cnt;
        while (true) {
            read_cnt = ifile.readsome(buf, BUF_SIZE);
            if (read_cnt < BUF_SIZE) {
                asio::write(sock, asio::buffer(buf, read_cnt));
                break;
            }
            asio::write(sock, asio::buffer(buf, BUF_SIZE));
        }

        sock.shutdown(asio::ip::tcp::socket::shutdown_send);
        read_cnt = sock.read_some(asio::buffer(buf, BUF_SIZE));
        std::cout << "Message from client: " << std::string_view(buf, read_cnt) << std::endl;
	}
	catch (std::system_error &e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
}

int calculate(int opnum, int opnds[], char op)
{
	int result=opnds[0], i;
	
	switch(op)
	{
	case '+':
		for(i=1; i<opnum; i++) result+=opnds[i];
		break;
	case '-':
		for(i=1; i<opnum; i++) result-=opnds[i];
		break;
	case '*':
		for(i=1; i<opnum; i++) result*=opnds[i];
		break;
	}
	return result;
}

