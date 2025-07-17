#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <thread>
#include <system_error>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include <asio.hpp>

void read_childproc(int sig);

void signal_handler(asio::signal_set& signals, const std::error_code& ec, int signo); 

int main(int argc, char* argv[])
{
    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    struct sigaction act;
    act.sa_handler=read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    int state=sigaction(SIGCHLD, &act, 0);

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

        const int BUF_SIZE = 1024;
        char message[BUF_SIZE];
        pid_t pid;
        std::error_code ec;
        while (true) {
            // Step 6. Creating an active socket.
            asio::ip::tcp::socket sock(io);

            asio::ip::tcp::endpoint remote_endpoint;
            // Step 7. Processing the next connection request and 
            // connecting the active socket to the client.
            acceptor.accept(sock, remote_endpoint, ec);
            if (ec) {
                if (ec == asio::error::interrupted) {
                    ec = std::error_code{};
                    continue;
                } else {
                    throw std::system_error(ec);
                }
            }

            std::cout << "new client connected..." << std::endl;

            pid=fork();
            if(pid==-1)
            {
                continue;
            }
            if(pid==0)
            {
                acceptor.close();
                // At this point 'sock' socket is connected to 
                //the client application and can be used to send data to
                // or receive data from it.
                int str_len=0;
                while ((str_len=sock.read_some(asio::buffer(message, BUF_SIZE), ec)) != 0) {
                    asio::write(sock, asio::buffer(message, str_len));
                }

                std::cout << "client disconnected..." << std::endl;
                return 0;
            }
            else
                sock.close();
        }
    }
    catch (std::system_error &e) {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what() << std::endl << std::endl;

        return e.code().value();
    }

    return 0;
}

void read_childproc(int sig)
{
    pid_t pid;
    int status;
    pid=waitpid(-1, &status, WNOHANG);
    printf("removed proc id: %d \n", pid);
}

void signal_handler(asio::signal_set& signals, const std::error_code& ec, int signo) {
    if (ec) {
        std::cout << "in signal_handler: " << ec.message() << std::endl;
        return;
    }

    if (signo == SIGCHLD) {
        read_childproc(signo);
    }

    signals.async_wait([&signals](const std::error_code& ec, int signo) {
            signal_handler(signals, ec, signo);
            });
}

