#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>
#include <string_view>

#include <asio.hpp>

int main(int argc, char* argv[])
{
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// Step 1. Here we assume that the server application has
	// already obtained the protocol port number.
	unsigned short port_num = std::stoi(argv[1]);

	// Step 2. Creating an endpoint.
	asio::ip::udp::endpoint ep(asio::ip::address_v4::any(),
		port_num);

	// Used by 'socket' class constructor.
	asio::io_context io;

	// Step 3. Creating and opening a socket.
	asio::ip::udp::socket sock(io, ep.protocol());

	std::error_code ec;

	// Step 4. Binding the socket to an endpoint.
	sock.bind(ep, ec);

	// Handling errors if any.
	if (ec) {
		// Failed to bind the socket. Breaking execution.
		std::cout << "Failed to bind the socket."
			<< "Error code = " << ec.value() << ". Message: "
			<< ec.message();

		return ec.value();
	}

    const int BUF_SIZE = 1024;
	char message[BUF_SIZE];
    int str_len;
    asio::ip::udp::endpoint remote_endpoint;
    for (int i=0; i<3; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        str_len = sock.receive_from(asio::buffer(message, BUF_SIZE), remote_endpoint);
        std::cout << "Message " << i+1 << ": " << std::string_view(message, str_len) << std::endl;
    }

	return 0;
}

