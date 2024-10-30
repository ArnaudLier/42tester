NAME ::= main
SRCS ::= main.c
OBJS ::= $(SRCS:.c=.o)
OBJS_LIBS ::= ft_printf/libftprintf.a
CFLAGS ::= -g3 -Wall -Wextra -Werror -Ift_printf

all: $(NAME)

$(NAME): $(OBJS) $(OBJS_LIBS)
	if [ -f "ft_printf/libft/libft.a" ]; then\
		$(CC) $(CFLAGS) -Ift_printf/libft $(OBJS) $(OBJS_LIBS) ft_printf/libft/libft.a -o $(NAME);\
	else\
		$(CC) $(CFLAGS) $(OBJS) $(OBJS_LIBS) -o $(NAME);\
	fi

ft_printf/libftprintf.a:
	cd ft_printf && make

clean:
	cd ft_printf && make clean
	rm -f $(OBJS)

fclean: clean
	cd ft_printf && make fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean ft_printf/libftprintf.a fclean re
