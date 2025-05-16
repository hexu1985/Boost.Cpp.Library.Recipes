#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

#include <boost/asio.hpp>

using namespace boost;

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

        const int BUF_SIZE = 1024;
        char opinfo[BUF_SIZE];
        for (int i=0; i<5; i++) {
            // Step 6. Creating an active socket.
            asio::ip::tcp::socket sock(io);

            // Step 7. Processing the next connection request and 
            // connecting the active socket to the client.
            acceptor.accept(sock);

            // At this point 'sock' socket is connected to 
            //the client application and can be used to send data to
            // or receive data from it.
            int opnd_cnt = 0;
            asio::read(sock, asio::buffer(&opnd_cnt, 1));

            const int OPSZ = 4;
            asio::read(sock, asio::buffer(opinfo, opnd_cnt*OPSZ+1));
            int result = calculate(opnd_cnt, (int*) opinfo, opinfo[opnd_cnt*OPSZ]);
            asio::write(sock, asio::buffer((char*) &result, sizeof(result)));
        } 
	}
	catch (system::system_error &e) {
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

