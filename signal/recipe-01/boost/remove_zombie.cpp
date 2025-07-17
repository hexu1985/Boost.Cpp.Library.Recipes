#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <iostream>
#include <thread>
#include <boost/asio.hpp>

using namespace boost;

void read_childproc(int sig)
{
	int status;
	pid_t id=waitpid(-1, &status, WNOHANG);
	if(WIFEXITED(status))
	{
        std::cerr <<  "Removed proc id: " << id << "\n";
        std::cerr << "Child send: " << WEXITSTATUS(status) << "\n";
	}
}

void signal_handler(asio::signal_set& signals, const system::error_code& ec, int signo) {
    if (ec) {
        std::cout << "in signal_handler: " << ec.message() << std::endl;
        return;
    }

    if (signo == SIGCHLD) {
        read_childproc(signo);
    } 

    signals.async_wait([&signals](const system::error_code& ec, int signo) {
            signal_handler(signals, ec, signo);
            });
}

int main() {
    asio::io_context io;
    asio::signal_set signals(io, SIGCHLD);

    signals.async_wait([&signals, &io](const system::error_code& ec, int signo) {
            signal_handler(signals, ec, signo);
    });

    std::thread io_thread([&io]() { io.run(); });
	auto pid=fork();
	if(pid==0)
	{
		puts("Hi! I'm child process");
		sleep(10);
        quick_exit(12);
	}
	else
	{
		printf("Child proc id: %d \n", pid);
		pid=fork();
		if(pid==0)
		{
			puts("Hi! I'm child process");
			sleep(10);
			quick_exit(24);
		}
		else
		{
			int i;
			printf("Child proc id: %d \n", pid);
			for(i=0; i<5; i++)
			{
				puts("wait...");
				sleep(5);
			}
            io.stop();
            io_thread.join();
		}
	}

	return 0;
}
