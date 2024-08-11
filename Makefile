OUT_DIR = bin
SRC_DIR = src

# SSL
SSL_LIBS_DIR = /opt/homebrew/opt/openssl@1.1/lib
SSL_INCLUDE_DIRS = /opt/homebrew/opt/openssl@1.1/include

# Compiler flags
CC = gcc
LIBS = -lpthread -L$(SSL_LIBS_DIR)
INCLUDE_DIRS = -I$(SSL_INCLUDE_DIRS)

all:
	make mkdir
	make server
	make client

mkdir:
	mkdir -p $(OUT_DIR)
server:
	$(eval OUTPUT := -o $(OUT_DIR)/server)
	$(CC) $(OUTPUT) \
	${INCLUDE_DIRS} \
	${LIBS} \
	$(SRC_DIR)/server.c
client:
	$(eval OUTPUT := -o $(OUT_DIR)/client)
	$(CC) $(OUTPUT) \
	${INCLUDE_DIRS} \
	${LIBS} \
	$(SRC_DIR)/client.c
clean:
	rm -rf $(OUT_DIR)