#include <thread>
#include <iostream>
#include <asio.hpp>

void timeout(int sig)
{
	if(sig==SIGALRM)
        std::cerr << "Time out!\n";
	alarm(2);	
}

void signal_handler(asio::signal_set& signals, const std::error_code& ec, int signo, int i) {
    if (ec) {
        std::cout << "in signal_handler: " << ec.message() << std::endl;
        return;
    }

    if (signo == SIGALRM) {
        timeout(signo);
    } 

    if (i < 3) {
        signals.async_wait([&signals, i](const std::error_code& ec, int signo) {
                signal_handler(signals, ec, signo, i+1);
                });

        std::cerr << "wait...\n";
    }
}

int main() {
    asio::io_context io;
    asio::signal_set signals(io, SIGALRM, SIGINT);

    signals.async_wait([&signals, &io](const std::error_code& ec, int signo) {
            signal_handler(signals, ec, signo, 1);
    });

	alarm(2);

    std::cerr << "wait...\n";
    io.run();

    return 0;
}

