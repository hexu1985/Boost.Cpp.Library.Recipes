#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>

using namespace boost;

typedef std::shared_ptr<asio::ip::tcp::socket> socket_ptr;
typedef std::shared_ptr<asio::ip::tcp::endpoint> endpoint_ptr; 

void handle_accept(asio::io_context& io, asio::ip::tcp::acceptor& acceptor, 
        socket_ptr sock, endpoint_ptr remote_endpoint,
        const boost::system::error_code& ec);

void start_accept(asio::io_context& io, asio::ip::tcp::acceptor& acceptor) {
    socket_ptr sock(new asio::ip::tcp::socket(io));
    endpoint_ptr remote_endpoint(new asio::ip::tcp::endpoint);
    acceptor.async_accept(*sock, *remote_endpoint,
            [&io, &acceptor, sock, remote_endpoint](const boost::system::error_code& ec) {
                handle_accept(io, acceptor, sock, remote_endpoint, ec);
            });
}

void handle_accept(asio::io_context& io, asio::ip::tcp::acceptor& acceptor, 
        socket_ptr sock, endpoint_ptr remote_endpoint,
        const boost::system::error_code& ec) {
    if (ec) {
        std::cout << "accept error: " << ec.message() << std::endl;
        return;
    }

    std::cout << "Connected client " << *remote_endpoint << std::endl;
	// 从这里开始, 你可以从socket读取或者写入
	start_accept(io, acceptor);
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


