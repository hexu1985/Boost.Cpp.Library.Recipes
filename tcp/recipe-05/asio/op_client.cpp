#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <system_error>

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

        const int BUF_SIZE = 1024;
        char opmsg[BUF_SIZE];

        std::cout << "Operand count: ";
        int opnd_cnt;
        std::cin >> opnd_cnt;
        opmsg[0]=(char)opnd_cnt;

        const int OPSZ = sizeof(int); 
        for(int i=0; i<opnd_cnt; i++)
        {
            int opnd = 0;
            std::cout << "Operand " << i+1 << ": ";
            std::cin >> opnd;
            memcpy(&opmsg[i*OPSZ+1], &opnd, OPSZ);
        }

        std::cout << "Operator： ";
        std::cin >> opmsg[opnd_cnt*OPSZ+1];
        asio::write(sock, asio::buffer(opmsg, opnd_cnt*OPSZ+2));
        int result;
        const int RLT_SIZE = sizeof(int);
        asio::read(sock, asio::buffer(&result, RLT_SIZE));
        std::cout << "Operation result: " << result << std::endl;
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
