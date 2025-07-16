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
        asio::ip::udp::endpoint
            ep(asio::ip::make_address(raw_ip_address),
                    port_num);

        asio::io_context io;

        // Step 3. Creating and opening a socket.
        asio::ip::udp::socket sock(io, ep.protocol());

        std::string message;
        const int BUF_SIZE = 1024;
        char buf[BUF_SIZE];
        int str_len = 0;
        asio::ip::udp::endpoint remote_endpoint;
        while (true) {
            std::cout << "Input message(Q to quit): ";
            std::getline(std::cin, message);

            if (message.length() ==1 && (message[0] == 'q' || message[0] == 'Q')) {
                break;
            }

            sock.send_to(asio::buffer(message), ep);
            str_len = sock.receive_from(asio::buffer(buf, BUF_SIZE), remote_endpoint);
            std::cout << "Message from server: " << std::string_view(buf, str_len) << std::endl;
        }
    }
    catch (std::system_error &e) {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what() << std::endl;

        return e.code().value();
    }

    return 0;
}
