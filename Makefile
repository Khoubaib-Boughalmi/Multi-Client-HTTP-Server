CC=gcc
CLIENT_NAME=CLIENT
SERVER_NAME=SERVER
CFLAGS=-Wall -Wextra -Werror -fsanitize=address

CLIENT_SRC=	client.c \
			sharedUtils.c \
			clientUtils.c \

SERVER_SRC=	server.c \
			sharedUtils.c \
			serverUtils.c \

CLIENT_OBJ=$(CLIENT_SRC:.c=.o)
SERVER_OBJ=$(SERVER_SRC:.c=.o)

all: $(CLIENT_NAME) $(SERVER_NAME)

$(CLIENT_NAME): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(CLIENT_OBJ) -o $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(SERVER_OBJ) -o $(SERVER_NAME)

clean:	
	rm -f $(CLIENT_OBJ) $(SERVER_OBJ)

fclean: clean
	rm -f $(CLIENT_NAME) $(SERVER_NAME)

re: fclean all	

.PHONY: all clean fclean re
