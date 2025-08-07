#include <stdio.h>
#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>

#define BUF_SIZE 30

using namespace boost;

void on_timer_timeout(asio::system_timer& timer, std::chrono::seconds interval, const boost::system::error_code& ec); 

void start_read_stdin(asio::posix::stream_descriptor& input, std::array<char, BUF_SIZE>& buf, 
        asio::system_timer& timer, std::chrono::seconds interval);

void on_read_stdin(asio::posix::stream_descriptor& input, std::array<char, BUF_SIZE>& buf, 
        asio::system_timer& timer, std::chrono::seconds interval, 
        const boost::system::error_code& ec, std::size_t length) {
    if (!ec) {
        timer.cancel();

        // 处理读取到的数据
        std::cout << "message from console: " << std::string(buf.data(), length);

        // 继续读取
        start_read_stdin(input, buf, timer, interval);
    } else if (ec != boost::asio::error::operation_aborted) {
        std::cerr << "Read error: " << ec.message() << std::endl;
    }
}

void start_timer(asio::system_timer& timer, std::chrono::seconds interval) {
    timer.expires_after(interval);
    timer.async_wait([&timer, interval](const boost::system::error_code& ec) {
                on_timer_timeout(timer, interval, ec);
            });
}

void on_timer_timeout(asio::system_timer& timer, std::chrono::seconds interval, const boost::system::error_code& ec) {
    if (!ec) {
        std::cout << "Time-out!" << std::endl;
        start_timer(timer, interval);
    } else if (ec == asio::error::operation_aborted) {
        // cancel
        return;
    } else {
        std::cout << "unexpected" << std::endl;
    }
}

void start_read_stdin(asio::posix::stream_descriptor& input, std::array<char, BUF_SIZE>& buf, 
        asio::system_timer& timer, std::chrono::seconds interval) {
    start_timer(timer, interval);
    input.async_read_some(asio::buffer(buf), 
            [&input, &buf, &timer, interval] (const boost::system::error_code& ec, std::size_t length) {
                on_read_stdin(input, buf, timer, interval, ec, length);
            });
}

int main() {
    asio::io_context io;

    asio::posix::stream_descriptor input{io, dup(STDIN_FILENO)};
    asio::system_timer timer(io);

    std::array<char, BUF_SIZE> buf;

    start_read_stdin(input, buf, timer, std::chrono::seconds(5));

    io.run();

    return 0;
}
