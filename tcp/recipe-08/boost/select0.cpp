#include <stdio.h>
#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>

#define BUF_SIZE 30

using namespace boost;

void input_read_callback(asio::posix::stream_descriptor& input, std::array<char, BUF_SIZE>& buf, const boost::system::error_code& ec, std::size_t length) {
    if (!ec) {
        // 处理读取到的数据
        std::cout << "message from console: " << std::string(buf.data(), length);

        // 继续读取
        input.async_read_some(asio::buffer(buf), 
                [&input, &buf](const boost::system::error_code& ec, std::size_t length) {
                    input_read_callback(input, buf, ec, length);
                });
    } else if (ec != boost::asio::error::operation_aborted) {
        std::cerr << "Read error: " << ec.message() << std::endl;
    }
}

int main() {
    asio::io_context io;

    asio::posix::stream_descriptor input{io, dup(STDIN_FILENO)};

    std::array<char, BUF_SIZE> buf;

    input.async_read_some(asio::buffer(buf),
            [&input, &buf](const boost::system::error_code& ec, std::size_t length) {
                input_read_callback(input, buf, ec, length);
            });

    io.run();

    return 0;
}
