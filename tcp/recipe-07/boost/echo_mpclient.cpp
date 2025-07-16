#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <unistd.h>

#include <boost/asio.hpp>

using namespace boost;

void read_routine(asio::ip::tcp::socket& sock);
void write_routine(asio::ip::tcp::socket& sock);

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
            ep(asio::ip::make_address(raw_ip_address),
                    port_num);

        asio::io_context io;

        // Step 3. Creating and opening a socket.
        asio::ip::tcp::socket sock(io, ep.protocol());

        // Step 4. Connecting a socket.
        sock.connect(ep);
        std::cout << "Connected..........." << std::endl;

        pid_t pid=fork();
        if(pid==0)
            write_routine(sock);
        else 
            read_routine(sock);
    }
    // Overloads of asio::ip::address::from_string() and 
    // asio::ip::tcp::socket::connect() used here throw
    // exceptions in case of error condition.
    catch (system::system_error &e) {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what() << std::endl;

        return e.code().value();
    }

    return 0;
}

void read_routine(asio::ip::tcp::socket& sock)
{
    const int BUF_SIZE = 1024;
    char buf[BUF_SIZE];
	while(true)
	{
        system::error_code ec;
        int str_len = sock.read_some(asio::buffer(buf, BUF_SIZE), ec);
		if(str_len==0)
			return;

        std::cout << "Message from server: " << std::string_view(buf, str_len) << std::endl;
	}
}

void write_routine(asio::ip::tcp::socket& sock)
{
    std::string message;
	while(true)
	{
        std::getline(std::cin, message);
        if (message.length() ==1 && (message[0] == 'q' || message[0] == 'Q')) {
            sock.shutdown(asio::ip::tcp::socket::shutdown_send);
            return;
        }

        asio::write(sock, asio::buffer(message));
	}
}
