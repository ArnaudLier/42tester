PROJECT_DIR ::= get_next_line
SRCS ::= mandatory.c $(PROJECT_DIR)/get_next_line.c $(PROJECT_DIR)/get_next_line_utils.c
OBJS ::= $(SRCS:.c=.o)
SRCS_B ::= bonus.c $(PROJECT_DIR)/get_next_line_bonus.c $(PROJECT_DIR)/get_next_line_utils_bonus.c
OBJS_B ::= $(SRCS_B:.c=.o)
BUFFER_SIZE ::= 10000000
CFLAGS ::= -g3 -Wall -Wextra -Werror -D BUFFER_SIZE=$(BUFFER_SIZE) -I$(PROJECT_DIR)

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
