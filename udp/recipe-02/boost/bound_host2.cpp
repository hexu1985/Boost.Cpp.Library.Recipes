#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>

#include <boost/asio.hpp>

using namespace boost;

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
        asio::ip::udp::endpoint
            ep(asio::ip::address::from_string(raw_ip_address),
                    port_num);

        asio::io_context io;

        // Step 3. Creating and opening a socket.
        asio::ip::udp::socket sock(io, ep.protocol());

        asio::ip::udp::endpoint remote_endpoint;

        std::string_view msg1 = "Hi!";
        std::string_view msg2 = "I'm another UDP host!";
        std::string_view msg3 = "Nice to meet you";
        sock.send_to(asio::buffer(msg1), ep);
        sock.send_to(asio::buffer(msg2), ep);
        sock.send_to(asio::buffer(msg3), ep);
    }
    catch (system::system_error &e) {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();

        return e.code().value();
    }

    return 0;
}
