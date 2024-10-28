NAME ::= main
SRCS ::= main.c
OBJS ::= $(SRCS:.c=.o)
OBJS_LIBS ::= ft_printf/libft/libft.a ft_printf/libftprintf.a
CFLAGS ::= -g3 -Wall -Wextra -Werror -Ift_printf -Ift_printf/libft

all: $(NAME)

$(NAME): $(OBJS) $(OBJS_LIBS)
	cc $(CFLAGS) $(OBJS) $(OBJS_LIBS) -o $(NAME)

ft_printf/libftprintf.a:
	cd ft_printf && make

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean ft_printf/libftprintf.a fclean re
