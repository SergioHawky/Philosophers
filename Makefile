NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -I include

SRC = src/main.c \
		src/init.c \
		src/routine.c \
		src/check.c \
		src/utils.c 

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "Compiled"

clean:
	rm -f src/*.o

fclean: clean
	rm -f $(NAME)
	@echo "Full clean"

re: fclean all

.PHONY: all clean fclean re
