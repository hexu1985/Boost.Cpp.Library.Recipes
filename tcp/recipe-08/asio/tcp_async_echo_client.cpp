#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <memory>
#include <system_error>

#include <asio.hpp>

typedef std::shared_ptr<asio::ip::tcp::socket> socket_ptr;
typedef std::shared_ptr<asio::posix::stream_descriptor> descriptor_ptr;
typedef asio::streambuf buffer_type;
typedef std::shared_ptr<buffer_type> buffer_ptr;

void do_read_from_console(socket_ptr sock, descriptor_ptr input, buffer_ptr network_buffer, buffer_ptr input_buffer);
void do_read_from_network(socket_ptr sock, descriptor_ptr input, buffer_ptr network_buffer, buffer_ptr input_buffer);

void close(socket_ptr sock, descriptor_ptr input) {
    sock->close();
    input->close();
}

void on_write_to_network(socket_ptr sock, descriptor_ptr input, 
        buffer_ptr network_buffer, buffer_ptr input_buffer,
        const std::error_code& ec, std::size_t bytes) {
    // check error
    if (ec) {
        std::cout << "write to network error: " << ec.message() << std::endl;
        close(sock, input);
        return;
    }

    do_read_from_network(sock, input, network_buffer, input_buffer);
}

void do_write_to_network(socket_ptr sock, descriptor_ptr input, 
        buffer_ptr network_buffer, buffer_ptr input_buffer, std::shared_ptr<std::string> message) {
    asio::async_write(*sock, asio::buffer(*message),
            [sock, input, network_buffer, input_buffer, message](const std::error_code& ec, std::size_t bytes) {
                on_write_to_network(sock, input, network_buffer, input_buffer, ec, bytes);
            });
}

void on_read_from_console(socket_ptr sock, descriptor_ptr input, 
        buffer_ptr network_buffer, buffer_ptr input_buffer,
        const std::error_code& ec, std::size_t bytes) {
    // check error
    if (ec) {
        std::cout << "read from  error: " << ec.message() << std::endl;
        close(sock, input);
        return;
    }

    std::istream is(input_buffer.get());
    std::string message;
    std::getline(is, message);
    message += '\n';

    // 检查是否退出
    if (message == "q\n" || message == "Q\n") {
        std::cout << "Exiting..." << std::endl;
        close(sock, input);
        return;
    }

    do_write_to_network(sock, input, network_buffer, input_buffer, std::make_shared<std::string>(message));
}

void on_read_from_network(socket_ptr sock, descriptor_ptr input, 
        buffer_ptr network_buffer, buffer_ptr input_buffer,
        const std::error_code& ec, std::size_t bytes) {
    // check error
    if (ec) {
        std::cout << "read from network error: " << ec.message() << std::endl;
        close(sock, input);
        return;
    }

    std::istream is(network_buffer.get());
    std::string message;
    std::getline(is, message);
    std::cout << "Message from server: " << message << std::endl;

    do_read_from_console(sock, input, network_buffer, input_buffer);
}

void do_read_from_console(socket_ptr sock, descriptor_ptr input, 
        buffer_ptr network_buffer, buffer_ptr input_buffer) {
    asio::async_read_until(*input, *input_buffer, '\n',
            [sock, input, network_buffer, input_buffer](const std::error_code& ec, std::size_t bytes) {
                on_read_from_console(sock, input, network_buffer, input_buffer, ec, bytes);
            });
}

void do_read_from_network(socket_ptr sock, descriptor_ptr input, 
        buffer_ptr network_buffer, buffer_ptr input_buffer) {
    asio::async_read_until(*sock, *network_buffer, '\n',
            [sock, input, network_buffer, input_buffer](const std::error_code& ec, std::size_t bytes) {
                on_read_from_network(sock, input, network_buffer, input_buffer, ec, bytes);
            });
}

void start_session(socket_ptr sock, descriptor_ptr input) {
    buffer_ptr network_buffer(new buffer_type);
    buffer_ptr input_buffer(new buffer_type);
    do_read_from_console(sock, input, network_buffer, input_buffer);
}

void on_connect(socket_ptr sock, descriptor_ptr input, const std::error_code& ec) {
    if (ec) {
        std::cout << "connect error: " << ec.message() << std::endl;
        close(sock, input);
        return;
    }

    std::cout << "Connected..........." << std::endl;
    std::cout << "Input message(Q to quit): " << std::flush;

    start_session(sock, input);
}

void start_connect(const asio::ip::tcp::endpoint& ep, 
        socket_ptr sock, descriptor_ptr input) {
    sock->async_connect(ep, 
            [sock, input](const std::error_code& ec) {
                on_connect(sock, input, ec);
            });
}

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
        socket_ptr sock{new asio::ip::tcp::socket(io, ep.protocol())};
        descriptor_ptr input{new asio::posix::stream_descriptor(io, ::dup(STDIN_FILENO))};

        start_connect(ep, sock, input);

        io.run();
    }
    // Overloads of asio::ip::address::from_string() and 
    // asio::ip::tcp::socket::connect() used here throw
    // exceptions in case of error condition.
    catch (std::system_error &e) {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what() << std::endl;

        return e.code().value();
    }

    return 0;
}

