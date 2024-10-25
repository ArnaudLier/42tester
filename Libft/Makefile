mandatory: libft/libft.h libft/libft.a main.c
	cc -g3 -Wall -Wextra -Werror main.c libft/libft.a -lbsd -Ilibft -o mandatory

libft/libft.a:
	cd libft && make

bonus: libft/libft.h libft/libft.a main.c bonus.c
	cd libft && make bonus
	cc -g3 -Wall -Wextra -Werror -DBONUS main.c bonus.c libft/libft.a -Ilibft -lbsd -o bonus
