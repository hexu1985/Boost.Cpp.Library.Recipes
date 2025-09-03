#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>

#define BUF_SIZE 100

using namespace boost;

typedef std::shared_ptr<asio::ip::tcp::socket> socket_ptr;
typedef std::shared_ptr<asio::ip::tcp::endpoint> endpoint_ptr; 
typedef std::array<char, BUF_SIZE> buffer_type;
typedef std::shared_ptr<buffer_type> buffer_ptr;

void handle_accept(asio::io_context& io, asio::ip::tcp::acceptor& acceptor, 
        socket_ptr sock, endpoint_ptr remote_endpoint,
        const system::error_code& ec);

void start_session(socket_ptr sock);

void do_read(socket_ptr sock, buffer_ptr read_buffer, buffer_ptr write_buffer);

void do_write(socket_ptr sock, buffer_ptr read_buffer, buffer_ptr write_buffer, size_t bytes);

void start_accept(asio::io_context& io, asio::ip::tcp::acceptor& acceptor) {
    socket_ptr sock(new asio::ip::tcp::socket(io));
    endpoint_ptr remote_endpoint(new asio::ip::tcp::endpoint);
    acceptor.async_accept(*sock, *remote_endpoint,
            [&io, &acceptor, sock, remote_endpoint](const system::error_code& ec) {
                handle_accept(io, acceptor, sock, remote_endpoint, ec);
            });
}

void handle_accept(asio::io_context& io, asio::ip::tcp::acceptor& acceptor, 
        socket_ptr sock, endpoint_ptr remote_endpoint,
        const system::error_code& ec) {
    if (ec) {
        std::cout << "accept error: " << ec.message() << std::endl;
        return;
    }

    std::cout << "Connected client " << *remote_endpoint << std::endl;
	// 从这里开始, 你可以从socket读取或者写入
    start_session(sock);

	start_accept(io, acceptor);
}

void start_session(socket_ptr sock) {
    buffer_ptr read_buffer(new buffer_type);
    buffer_ptr write_buffer(new buffer_type);
    do_read(sock, read_buffer, write_buffer);
}

void on_read(socket_ptr sock, buffer_ptr read_buffer, buffer_ptr write_buffer, 
        system::error_code ec, size_t bytes) {
    if (ec) {
        std::cout << "read error: " << ec.message() << std::endl;
        if (ec == asio::error::eof || ec == asio::error::connection_reset) {
            std::cout << "client disconnected..." << std::endl;
        }
        sock->close();
        return;
    }

    do_write(sock, read_buffer, write_buffer, bytes);
}

void do_read(socket_ptr sock, buffer_ptr read_buffer, buffer_ptr write_buffer) {
    sock->async_read_some(asio::buffer(*read_buffer),
            [sock, read_buffer, write_buffer](system::error_code ec, std::size_t bytes) {
                on_read(sock, read_buffer, write_buffer, ec, bytes);
            });
}

void on_write(socket_ptr sock, buffer_ptr read_buffer, buffer_ptr write_buffer, 
        system::error_code ec, size_t bytes) {
    if (ec) {
        std::cout << "write error: " << ec.message() << std::endl;
        sock->close();
        return;
    }

    do_read(sock, read_buffer, write_buffer);
}

void do_write(socket_ptr sock, buffer_ptr read_buffer, buffer_ptr write_buffer, size_t bytes) {
    memcpy(write_buffer->data(), read_buffer->data(), bytes);
    asio::async_write(*sock, asio::buffer(write_buffer->data(), bytes),
            [sock, read_buffer, write_buffer](const system::error_code& ec, std::size_t bytes) {
                on_write(sock, read_buffer, write_buffer, ec, bytes);
            });
}

int main(int argc, char* argv[])
{
    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // The size of the queue containing the pending connection
    // requests.
    const int BACKLOG_SIZE = 5;

    // Step 1. Here we assume that the server application has
    // already obtained the protocol port number.
    unsigned short port_num = std::stoi(argv[1]);

    // Step 2. Creating a server endpoint.
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(),
            port_num);

    asio::io_context io;

    try {
        // Step 3. Instantiating and opening an acceptor socket.
        asio::ip::tcp::acceptor acceptor(io, ep.protocol());

        acceptor.set_option(asio::socket_base::reuse_address(true));

        // Step 4. Binding the acceptor socket to the 
        // server endpint.
        acceptor.bind(ep);

        // Step 5. Starting to listen to incoming connection
        // requests.
        acceptor.listen(BACKLOG_SIZE);

        start_accept(io, acceptor);

        io.run();

    }
    catch (system::system_error &e) {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what() << std::endl;

        return e.code().value();
    }

    return 0;
}


