OUT_DIR = bin
SRC_DIR = src

# SSL
SSL_LIBS_DIR = /opt/homebrew/opt/openssl@1.1/lib
SSL_INCLUDE_DIRS = /opt/homebrew/opt/openssl@1.1/include

# Compiler flags
CC = gcc
OPENSSL_LIBS = -L$(SSL_LIBS_DIR) -lssl -lcrypto
OPENSSL_INCLUDE_DIRS = -I$(SSL_INCLUDE_DIRS)

mkdir:
	mkdir -p $(OUT_DIR)
openssl:
	make mkdir
	make openssl-server
	make openssl-client
openssl-server:
	$(eval OUTPUT := -o $(OUT_DIR)/server-openssl)
	$(eval PROJECT_DIR := $(SRC_DIR)/openssl)
	$(CC) $(OUTPUT) \
	${OPENSSL_INCLUDE_DIRS} \
	${OPENSSL_LIBS} \
	$(PROJECT_DIR)/server.c
openssl-client:
	$(eval OUTPUT := -o $(OUT_DIR)/client-openssl)
	$(eval PROJECT_DIR := $(SRC_DIR)/openssl)
	$(CC) $(OUTPUT) \
	${OPENSSL_INCLUDE_DIRS} \
	${OPENSSL_LIBS} \
	$(PROJECT_DIR)/client.c
clean:
	rm -rf $(OUT_DIR)