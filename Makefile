build: libft.h libft.a main.c
	cc -g3 -Wall -Wextra -Werror main.c libft.a -lbsd -o main

bonus: libft.h libft.a main.c bonus.c
	cc -g3 -Wall -Wextra -Werror -DBONUS main.c bonus.c libft.a -lbsd -o main

.PHONY: build
