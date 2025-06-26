#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>
#include <string_view>

#include <boost/asio.hpp>

using namespace boost;

int main(int argc, char* argv[])
{
	if(argc!=3) {
		printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
		exit(1);
	 }

	// Step 1. Here we assume that the server application has
	// already obtained the protocol port number.
    std::string multicast_ip_address = argv[1];
    unsigned short port_num = std::stoi(argv[2]);

	// Step 2. Creating an endpoint.
	asio::ip::udp::endpoint ep(asio::ip::address_v4::any(),
		port_num);

	// Used by 'socket' class constructor.
	asio::io_context io;

	// Step 3. Creating and opening a socket.
	asio::ip::udp::socket sock(io, ep.protocol());

    sock.set_option(asio::socket_base::reuse_address(true));

	system::error_code ec;

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

    // Step 5. Joining the group of multicast.
    sock.set_option(
            asio::ip::multicast::join_group(
                asio::ip::make_address(
                    multicast_ip_address)));

    const int BUF_SIZE = 1024;
	char buf[BUF_SIZE];
    int str_len;
    asio::ip::udp::endpoint remote_endpoint;
    while (true) {
        str_len = sock.receive_from(asio::buffer(buf, BUF_SIZE), remote_endpoint);
        std::cout << std::string_view(buf, str_len) << std::flush;
    }

	return 0;
}

