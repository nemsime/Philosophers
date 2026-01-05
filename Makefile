CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
SRCS = ftlib.c philo.c valid_init.c gs.c threads_start.c utils.c
NAME = philo

OBJS = $(SRCS:.c=.o)

HEADER = philo.h

%.o: %.c $(HEADER) Makefile
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
