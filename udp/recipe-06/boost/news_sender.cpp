#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <thread>
#include <chrono>

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
            ep(asio::ip::make_address(raw_ip_address),
                    port_num);

        asio::io_context io;

        // Step 3. Creating and opening a socket.
        asio::ip::udp::socket sock(io, ep.protocol());

        const int TTL=64;
        sock.set_option(asio::ip::multicast::hops(TTL));

        std::ifstream ifile("news.txt");
        if (!ifile) {
            std::cout << "open news.txt failed" << std::endl;
            return -1;
        }

        std::string buf;
        while (std::getline(ifile, buf)) {
            buf += '\n';
            sock.send_to(asio::buffer(buf),ep);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (system::system_error &e) {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what() << std::endl;

        return e.code().value();
    }

    return 0;
}
