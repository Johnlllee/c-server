#ifndef __MYSSL_H__
#define __MYSSL_H__

#include <stdio.h>
#include <stdlib.h>
#include <openssl/ssl.h>
#include <openssl/crypto.h>

#define CHECK_SSL(ssl) \
    do { \
        if ((ssl) == NULL) { \
            fprintf(stderr, "SSL is NULL at %s:%d\n", __FILE__, __LINE__); \
            return -1; \
        } \
    } while (0)

typedef enum {
    SSL_SERVER,
    SSL_CLIENT
} SSL_MODE;

/*
* Initialize SSL context
* 
* @param cert_file: path to certificate file
* @param key_file: path to key file
* @param mode: SSL_SERVER or SSL_CLIENT
* @param sockfd: socket file descriptor (sockfd to be used with SSL)
* 
* @return SSL context
*/
SSL *init_ssl(char* cert_file, char* key_file, SSL_MODE mode, int sockfd)
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl = NULL;

    // Openssl_add_all_algorithms(); // deprecated, plz use OPENSSL_init_crypto
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | OPENSSL_INIT_ADD_ALL_CIPHERS | OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
    SSL_load_error_strings();

    if (mode == SSL_SERVER) {
        method = SSLv23_server_method();
    }
    else if (mode == SSL_CLIENT) {
        method = SSLv23_client_method();
    }
    else {
        fprintf(stderr, "Invalid SSL mode\n");
        return NULL;
    }

    // 1 - create new SSL context
    ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        printf("ssl_phase(create_ctx) new ctx failed\n");
        return NULL;
    }

    if ((SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0) || 
    (SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0))
    {
        printf("ssl_phase(create_ctx) use SSL_CTX_use_certificate_file or SSL_CTX_use_PrivateKey_file failed\n");
        return NULL;
    }

    // 2 - create new SSL
    ssl = SSL_new(ctx);
    if (ssl == NULL) {
        printf("ssl_phase(create_ssl) failed\n");
        return NULL;
    }

    // 3 - SSL bind fd
    if (SSL_set_fd(ssl, sockfd) == 0) {
        printf("ssl_phase(bind_fd) failed\n");
        return NULL;
    }

    // 4 - SSL handshake (connect & accept)
    if ((mode == SSL_CLIENT && SSL_connect(ssl) <= 0) || 
    (mode == SSL_SERVER && SSL_accept(ssl) <= 0))
    {
        printf("ssl_phase(handshake) failed\n");
        return NULL;
    }

    return ssl;

error_return:
    ERR_print_errors_fp(stderr);
    return NULL;
}

#endif // __MYSSL_H__