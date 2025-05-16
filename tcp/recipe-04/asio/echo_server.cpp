#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <system_error>

#include <asio.hpp>

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
        char message[BUF_SIZE];
        for (int i=0; i<5; i++) {
            // Step 6. Creating an active socket.
            asio::ip::tcp::socket sock(io);

            // Step 7. Processing the next connection request and 
            // connecting the active socket to the client.
            acceptor.accept(sock);

            std::cout << "Connected client " << i+1 << std::endl;

            // At this point 'sock' socket is connected to 
            //the client application and can be used to send data to
            // or receive data from it.
            int str_len=0;
            std::error_code ec;
            while ((str_len=sock.read_some(asio::buffer(message, BUF_SIZE), ec)) != 0) {
                asio::write(sock, asio::buffer(message, str_len));
        }
        } 
	}
	catch (std::system_error &e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
}


