/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alier <alier@student.42mulhouse.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:35:07 by alier             #+#    #+#             */
/*   Updated: 2024/10/28 18:35:41 by alier            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include "get_next_line.h"

bool	print_line(int fd)
{
	char	*line;
	size_t	i;

	line = get_next_line(fd);
	if (line == NULL)
	{
		printf("line was NULL\n");
		return (false);
	}
	i = 0;
	while (line[i])
	{
		if (line[i] != '\n')
			printf("%c", line[i]);
		else
			printf("\\n");
		i++;
	}
	printf("\n");
	free(line);
	return (true);
}

void	print_all_lines(int fd)
{
	size_t	i;

	i = 0;
	while (true)
	{
		printf("%3zu: ", i);
		if (!print_line(fd))
			break ;
		i++;
	}
}

int	main(void)
{
	int		test_fd;
	size_t	i;

	test_fd = open("test.txt", O_RDONLY);
	i = 0;
	while (true)
	{
		print_line(INT_MAX);
		if (i % 2 == 0)
			print_line(test_fd);
		else if (!print_line(STDIN_FILENO))
			break ;
		i++;
	}
	close(test_fd);
}
