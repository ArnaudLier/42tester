SRCS ::= mandatory.c get_next_line/get_next_line.c get_next_line/get_next_line_utils.c
OBJS ::= $(SRCS:.c=.o)
SRCS_B ::= bonus.c get_next_line/get_next_line_bonus.c get_next_line/get_next_line_utils_bonus.c
OBJS_B ::= $(SRCS_B:.c=.o)
BUFFER_SIZE ::= 42
CFLAGS ::= -g3 -Wall -Wextra -Werror -D BUFFER_SIZE=$(BUFFER_SIZE) -Iget_next_line

all: mandatory bonus

mandatory: $(OBJS)
	cc $(CFLAGS) $(OBJS) -o mandatory

bonus: $(OBJS_B)
	cc $(CFLAGS) $(OBJS_B) -o bonus

clean:
	rm -f $(OBJS) $(OBJS_B)

fclean: clean
	rm -f mandatory bonus

re: fclean all

.PHONY: all clean fclean re
