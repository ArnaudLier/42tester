NAME ::= main
SRCS ::= main.c
OBJS ::= $(SRCS:.c=.o)
CFLAGS ::= -g3 -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS) libft.a libftprintf.a
	cc $(CFLAGS) $(OBJS) libftprintf.a libft.a -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
