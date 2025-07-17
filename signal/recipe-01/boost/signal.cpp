#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <thread>
#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

void timeout(int sig)
{
	if(sig==SIGALRM)
        std::cerr << "Time out!\n";

	alarm(2);	
}

void keycontrol(int sig)
{
	if(sig==SIGINT)
        std::cerr << "CTRL+C pressed\n";
}

void signal_handler(asio::signal_set& signals, const std::error_code& ec, int signo) {
    static int i = 1;
    if (ec) {
        std::cout << "in signal_handler: " << ec.message() << std::endl;
        return;
    }

    if (signo == SIGALRM) {
        timeout(signo);
    } else if (signo == SIGINT) {
        keycontrol(signo);
    }

    if (i < 3) {
        signals.async_wait([&signals](const std::error_code& ec, int signo) {
                signal_handler(signals, ec, signo);
                });

        i++;
        std::cerr << "wait...\n";
    }
}

int main() {
    asio::io_context io;
    asio::signal_set signals(io, SIGALRM, SIGINT);

    signals.async_wait([&signals, &io](const std::error_code& ec, int signo) {
            signal_handler(signals, ec, signo);
    });

	alarm(2);

    std::cerr << "wait...\n";
    io.run();

    return 0;
}
