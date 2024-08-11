#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/err.h>


#define BUFFER_SIZE 1024
#define PORT 9090

int main()
{
    int listen_fd, connfd;
    struct sockaddr_in saddr, caddr;
    char buffer[BUFFER_SIZE];

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&saddr, 0, sizeof saddr);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(PORT);

    if(bind(listen_fd, (struct sockaddr *) &saddr, sizeof saddr) != 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    listen(listen_fd, 10);
    while(1) {
        socklen_t len = sizeof caddr;
        connfd = accept(listen_fd, (struct sockaddr *) &caddr, &len);
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t ret = read(connfd, buffer, BUFFER_SIZE);
        printf("received(%ld):\n%s\n", ret, buffer);

        char *response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, world</h1></body></html>\n";
        write(connfd, response, strlen(response));
        close(connfd);
    }

    return 0;
}