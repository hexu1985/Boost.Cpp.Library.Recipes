#include <stdio.h>
#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>

using namespace boost;

void on_read_stdin(asio::posix::stream_descriptor& input, asio::streambuf& buf, const system::error_code& ec, std::size_t length);

void start_read_stdin(asio::posix::stream_descriptor& input, asio::streambuf& buf) {
    asio::async_read_until(input, buf, '\n',
            [&input, &buf](const system::error_code& ec, std::size_t length) {
                on_read_stdin(input, buf, ec, length);
            });
}

void on_read_stdin(asio::posix::stream_descriptor& input, asio::streambuf& buf, const system::error_code& ec, std::size_t length) {
    if (!ec) {
        // 处理读取到的数据
        std::istream is(&buf);
        std::string message;
        std::getline(is, message);
        std::cout << "message from console: " << message << std::endl;

        // 继续读取
        start_read_stdin(input, buf);
    } else if (ec != asio::error::operation_aborted) {
        std::cerr << "Read error: " << ec.message() << std::endl;
    }
}

int main() {
    asio::io_context io;

    asio::posix::stream_descriptor input{io, dup(STDIN_FILENO)};

    asio::streambuf buf;

    start_read_stdin(input, buf);

    io.run();

    return 0;
}
