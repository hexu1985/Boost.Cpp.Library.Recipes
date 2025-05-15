#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

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

		// Step 6. Creating an active socket.
		asio::ip::tcp::socket sock(io);

		// Step 7. Processing the next connection request and 
		// connecting the active socket to the client.
		acceptor.accept(sock);

		// At this point 'sock' socket is connected to 
		//the client application and can be used to send data to
		// or receive data from it.
        std::string_view message = "Hello World!";

        asio::write(sock, asio::buffer(message));
	}
	catch (std::system_error &e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
}


