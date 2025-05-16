#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

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
        int str_len=0;
        int idx=0, read_len=0;
        system::error_code ec;
        while (read_len=sock.read_some(asio::buffer(&message[idx++], 1), ec)) {
            if (ec) {
                throw system::system_error(ec, "read() error!");
            }
            str_len+=read_len;
        }
        std::cout << "Message from server: " << std::string_view(message, str_len) << std::endl;
        std::cout << "Function read call count: " << str_len << std::endl;
    }
    // Overloads of asio::ip::address::from_string() and 
    // asio::ip::tcp::socket::connect() used here throw
    // exceptions in case of error condition.
    catch (system::system_error &e) {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();

        return e.code().value();
    }

    return 0;
}
