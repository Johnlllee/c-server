#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
// #include <openssl/err.h>

#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define PORT 9090

int main()
{
    int sockfd;
    struct sockaddr_in saddr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&saddr, 0, sizeof saddr);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    saddr.sin_port = htons(PORT);

    connect(sockfd, (struct sockaddr *)&saddr, sizeof saddr);

    char* msg = "GET / HTTP/1.1\nHost: localhost:9090\n\n";
    write(sockfd, msg, strlen(msg));

    memset(buffer, 0, BUFFER_SIZE);
    ssize_t ret = read(sockfd, buffer, BUFFER_SIZE);
    printf("received(%ld):\n%s\n", ret, buffer);
    close(sockfd);

    return 0;
}