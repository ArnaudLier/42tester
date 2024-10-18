NAME ::= main
SRCS ::= main.c get_next_line.c get_next_line_utils.c
OBJS ::= $(SRCS:.c=.o)
BUFFER_SIZE ::= 42
CFLAGS ::= -g3 -Wall -Wextra -Werror -D BUFFER_SIZE=$(BUFFER_SIZE) 

all: $(NAME)

$(NAME): $(OBJS)
	cc $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
