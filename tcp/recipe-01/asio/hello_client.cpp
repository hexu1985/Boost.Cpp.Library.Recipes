#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

#include <asio.hpp>

int main(int argc, char* argv[])
{
    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // Step 1. Assume that the client application has already
    // obtained the IP address and protocol port number of the
    // target server.
    std::string raw_ip_address = argv[1];
    unsigned short port_num = std::stoi(argv[2]);

    try {
        // Step 2. Creating an endpoint designating 
        // a target server application.
        asio::ip::tcp::endpoint
            ep(asio::ip::address::from_string(raw_ip_address),
                    port_num);

        asio::io_context io;

        // Step 3. Creating and opening a socket.
        asio::ip::tcp::socket sock(io, ep.protocol());

        // Step 4. Connecting a socket.
        sock.connect(ep);

        const unsigned char MESSAGE_SIZE = 30;
        char message[MESSAGE_SIZE];
		int str_len = sock.read_some(asio::buffer(message, MESSAGE_SIZE));
        std::cout << "Message from server: " << std::string_view(message, str_len) << std::endl;
    }
    // Overloads of asio::ip::address::from_string() and 
    // asio::ip::tcp::socket::connect() used here throw
    // exceptions in case of error condition.
    catch (std::system_error &e) {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();

        return e.code().value();
    }

    return 0;
}
