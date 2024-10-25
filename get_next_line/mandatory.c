/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandatory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alier <alier@student.42mulhouse.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:35:07 by alier             #+#    #+#             */
/*   Updated: 2024/10/25 12:03:23 by alier            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

	test_fd = open("test.txt", O_RDONLY);
	print_all_lines(test_fd);
	close(test_fd);
	print_line(test_fd);
	print_line(1000);
	print_line(-1);
	print_all_lines(STDIN_FILENO);
}
