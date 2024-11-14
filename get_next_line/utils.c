/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alier <alier@student.42mulhouse.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:38:01 by alier             #+#    #+#             */
/*   Updated: 2024/11/07 16:16:56 by alier            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <stddef.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <dlfcn.h>

bool	malloc_failure = false;

ssize_t read(int fd, void *buf, size_t count)
{
	ssize_t (*libc_read)(int, void *, size_t);

	libc_read = dlsym(RTLD_NEXT, "read");
	if (count != BUFFER_SIZE)
	{
		fprintf(stderr, "%zu was read instead of %zu\n", count, (size_t) BUFFER_SIZE);
		return (-1);
	}
	return (libc_read(fd, buf, count));
}

void *malloc(size_t size)
{
	void *(*libc_malloc)(size_t);

	if (malloc_failure)
		return (NULL);
	libc_malloc = dlsym(RTLD_NEXT, "malloc");
	return (libc_malloc(size));
}

