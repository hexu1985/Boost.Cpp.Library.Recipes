#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int send_sock;
    struct sockaddr_in remote_addr;
    FILE *fp;
    char buf[BUF_SIZE];

    if(argc!=3) {
        printf("Usage : %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }

    send_sock=socket(PF_INET, SOCK_DGRAM, 0);	
    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family=AF_INET;
    remote_addr.sin_addr.s_addr=inet_addr(argv[1]);
    remote_addr.sin_port=htons(atoi(argv[2]));

    if((fp=fopen("news.txt", "r"))==NULL)
        error_handling("fopen() error");

    while(!feof(fp))
    {
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 
                0, (struct sockaddr*)&remote_addr, sizeof(remote_addr));
        sleep(1);
    }

    close(send_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
