NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -I include

SRC = src/main.c \
		src/init.c \
		src/routine.c \
		src/monitor.c \
		src/utils.c 

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f src/*.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
