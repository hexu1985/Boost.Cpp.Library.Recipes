#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_addr;
    fd_set readfds;
    int max_fd;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");
	
    fputs("Input message(Q to quit): \n", stdout);
    while (1) 
    {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);  // 标准输入
        FD_SET(sock, &readfds);        // socket

        max_fd = (sock > STDIN_FILENO) ? sock : STDIN_FILENO;

        // 使用select监控文件描述符
        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) == -1) {
            perror("select");
            break;
        }

        // 检查标准输入是否有数据可读
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            if (fgets(message, BUF_SIZE, stdin) == NULL) {
                printf("EOF received, exiting...\n");
                break;
            }

            // 检查是否要退出
            if(!strcmp(message,"q\n") || !strcmp(message,"Q\n")) {
                printf("Exiting...\n");
                break;
            }

            // 发送数据到服务器
            if (write(sock, message, strlen(message)) == -1) {
                perror("write");
                break;
            }
        }

        // 检查socket是否有数据可读（服务器回应）
        if (FD_ISSET(sock, &readfds)) {
            str_len = read(sock, message, BUF_SIZE - 1);
            if (str_len == -1) {
                perror("read");
                break;
            } else if (str_len == 0) {
                printf("Server closed the connection\n");
                break;
            } else {
                message[str_len] = '\0';
                printf("Message from server: %s", message);
            }
        }
    }

	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
