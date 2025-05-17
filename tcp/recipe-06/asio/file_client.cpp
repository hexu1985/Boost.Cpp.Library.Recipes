#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>

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
        std::cout << "Connected..........." << std::endl;

        std::ofstream ofile("receive.dat", 
                std::ios::out | std::ios::binary | std::ios::trunc);
        if (!ofile) {
            std::cout << "open receive.dat failed!\n";
            return -1;
        }

        const int BUF_SIZE = 30;
        char buf[BUF_SIZE];
        int read_cnt;
        std::error_code ec;
        while ((read_cnt=sock.read_some(asio::buffer(buf, BUF_SIZE), ec)) != 0) {
            ofile.write(buf, read_cnt);
        }

        std::cout << "Received file data" << std::endl;
        asio::write(sock, asio::buffer("Thank you", 10));
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
