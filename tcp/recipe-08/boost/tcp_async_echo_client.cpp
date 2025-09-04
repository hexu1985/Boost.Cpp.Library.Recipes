#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>

using namespace boost;

typedef std::shared_ptr<asio::ip::tcp::socket> socket_ptr;
typedef std::shared_ptr<boost::asio::posix::stream_descriptor> descriptor_ptr;
typedef boost::asio::streambuf buffer_type;
typedef std::shared_ptr<buffer_type> buffer_ptr;

void do_read_from_console(socket_ptr sock, descriptor_ptr input, buffer_ptr write_buffer);
void do_read_from_network(socket_ptr sock, descriptor_ptr input, buffer_ptr read_buffer);

void close(socket_ptr sock, descriptor_ptr input) {
    sock->close();
    input->close();
}

void on_write_to_network(socket_ptr sock, descriptor_ptr input, 
        buffer_ptr write_buffer,
        const system::error_code& ec, std::size_t bytes) {
    // check error
    if (ec) {
        std::cout << "write to network error: " << ec.message() << std::endl;
        close(sock, input);
        return;
    }

    do_read_from_console(sock, input, write_buffer);
}

void do_write_to_network(socket_ptr sock, descriptor_ptr input, 
        buffer_ptr write_buffer) {
    boost::asio::async_write(*sock, *write_buffer,
            [sock, input, write_buffer](const system::error_code& ec, std::size_t bytes) {
                on_write_to_network(sock, input, write_buffer, ec, bytes);
            });
}

void on_read_from_console(socket_ptr sock, descriptor_ptr input, 
        buffer_ptr write_buffer,
        const system::error_code& ec, std::size_t bytes) {
    // check error
    if (ec) {
        std::cout << "read from console error: " << ec.message() << std::endl;
        close(sock, input);
        return;
    }

    // peek from write_buffer
    auto buffers = write_buffer->data();                              
    std::string_view message(
            static_cast<const char*>(asio::buffer(buffers).data()),
            asio::buffer_size(buffers)
            );

    // 检查是否退出
    if (message == "q\n" || message == "Q\n") {
        std::cout << "Exiting..." << std::endl;
        close(sock, input);
        return;
    }

    do_write_to_network(sock, input, write_buffer);
}

void on_read_from_network(socket_ptr sock, descriptor_ptr input, 
        buffer_ptr read_buffer,
        const system::error_code& ec, std::size_t bytes) {
    // check error
    if (ec) {
        std::cout << "read from console error: " << ec.message() << std::endl;
        close(sock, input);
        return;
    }

    std::istream is(read_buffer.get());
    std::string message;
    std::getline(is, message);
    std::cout << "Message from server: " << message << std::endl;

    do_read_from_network(sock, input, read_buffer);
}

void do_read_from_console(socket_ptr sock, descriptor_ptr input, buffer_ptr write_buffer) {
    asio::async_read_until(*input, *write_buffer, '\n',
            [sock, input, write_buffer](const system::error_code& ec, std::size_t bytes) {
                on_read_from_console(sock, input, write_buffer, ec, bytes);
            });
}

void do_read_from_network(socket_ptr sock, descriptor_ptr input, buffer_ptr read_buffer) {
    asio::async_read_until(*sock, *read_buffer, '\n',
            [sock, input, read_buffer](const system::error_code& ec, std::size_t bytes) {
                on_read_from_network(sock, input, read_buffer, ec, bytes);
            });
}

void start_session(socket_ptr sock, descriptor_ptr input) {
    buffer_ptr read_buffer(new buffer_type);
    buffer_ptr write_buffer(new buffer_type);
    do_read_from_console(sock, input, write_buffer);
    do_read_from_network(sock, input, read_buffer);
}

void on_connect(socket_ptr sock, descriptor_ptr input, const system::error_code& ec) {
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
            [sock, input](const system::error_code& ec) {
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
        descriptor_ptr input{new boost::asio::posix::stream_descriptor(io, ::dup(STDIN_FILENO))};

        start_connect(ep, sock, input);

        io.run();
#if 0
        // Step 4. Connecting a socket.
        sock.connect(ep);
        std::cout << "Connected..........." << std::endl;

        std::string message;
        const int BUF_SIZE = 1024;
        char buf[BUF_SIZE];
        int str_len = 0;
        while (true) {
            std::cout << "Input message(Q to quit): ";
            std::getline(std::cin, message);

            if (message.length() ==1 && (message[0] == 'q' || message[0] == 'Q')) {
                break;
            }

            asio::write(sock, asio::buffer(message));
            str_len = sock.read_some(asio::buffer(buf, BUF_SIZE));
            std::cout << "Message from server: " << std::string_view(buf, str_len) << std::endl;
        }
#endif
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

