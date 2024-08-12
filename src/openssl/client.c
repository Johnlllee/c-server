#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/err.h>

#include "myssl.h"

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

    SSL* ssl = init_ssl("./cert/cert.pem", "./cert/key.pem", SSL_CLIENT, sockfd); // use your own certificate and key file
    CHECK_SSL(ssl);

    char* msg = "GET / HTTP/1.1\nHost: localhost:9090\n\n";
    SSL_write(ssl, msg, strlen(msg));

    memset(buffer, 0, BUFFER_SIZE);
    ssize_t ret = SSL_read(ssl, buffer, BUFFER_SIZE);
    printf("received(%ld):\n%s\n", ret, buffer);
    close(sockfd);
    SSL_shutdown(ssl);
    SSL_free(ssl);

    return 0;
}