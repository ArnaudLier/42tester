/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alier <alier@student.42mulhouse.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:22:29 by alier             #+#    #+#             */
/*   Updated: 2024/10/30 10:44:40 by alier            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <fcntl.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <bsd/string.h>
#include <ctype.h>
#include "libft.h"
#include "main.h"
#include "bonus.h"

void	test_chrclass(t_chrclasser libc, t_chrclasser libft, const char *name)
{
	unsigned char	i;
	int				libft_result;
	int				libc_result;

	i = 0;
	printf("Testing Project Libft: %s.\n", name);
	while (true)
	{
		libc_result = libc(i);
		libft_result = libft(i);
		if ((libft_result && !libc_result)
			|| (!libft_result && libc_result))
		{
			fprintf(stderr, "for input %d\n", i);
			fprintf(stderr, "libc (%d) and libft (%d) don't agree",
				libc_result, libft_result);
			exit(EXIT_FAILURE);
		}
		if (i == UCHAR_MAX)
			break ;
		i++;
	}
}

void	test_strlen(void)
{
	const char	*test_values[8];
	size_t		count;
	size_t		i;
	size_t		libc_result;
	size_t		libft_result;

	test_values[0] = "1234";
	test_values[1] = " \t\n\v\f\r5";
	test_values[2] = "-42";
	test_values[3] = " -+-+-42";
	test_values[4] = "2147483647";
	test_values[5] = "-2147483648";
	test_values[6] = "eeeeeeeeeeeeeeeeeeeeeeeeeee"
		"eeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
	test_values[7] = "\0\0";
	count = sizeof(test_values) / sizeof(test_values[0]);
	i = 0;
	printf("Testing Project Libft: ft_strlen.\n");
	while (i < count)
	{
		libc_result = strlen(test_values[i]);
		libft_result = ft_strlen(test_values[i]);
		if (libc_result != libft_result)
		{
			fprintf(stderr, "for input %s", test_values[i]);
			fprintf(stderr, "libc: %zu != libft: %zu\n", libc_result,
				libft_result);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	test_memset(void)
{
	char		test_values[3];
	char		buffer[20];
	size_t		count;
	size_t		i;
	size_t		j;
	char		*return_value;
	size_t		bytes;

	bytes = sizeof(buffer) / sizeof(buffer[0]);
	test_values[0] = 'c';
	test_values[1] = '*';
	test_values[2] = '\0';
	count = sizeof(test_values) / sizeof(test_values[0]);
	i = 0;
	printf("Testing Project Libft: ft_memset.\n");
	while (i < count)
	{
		return_value = ft_memset(buffer, test_values[i], bytes);
		if (return_value != buffer)
		{
			fprintf(stderr, "memset didn't return input buffer");
			exit(EXIT_FAILURE);
		}
		j = 0;
		while (j < bytes)
		{
			if (buffer[j] != test_values[i])
			{
				fprintf(stderr, "memset didn't set a byte that should have been");
				exit(EXIT_FAILURE);
			}
			j++;
		}
		i++;
	}
}

void	test_bzero(void)
{
	char		buffer[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	size_t		i;
	size_t		bytes;

	bytes = sizeof(buffer) / sizeof(buffer[0]);
	printf("Testing Project Libft: ft_bzero.\n");
	ft_bzero(buffer, bytes);
	i = 0;
	while (i < bytes)
	{
		if (buffer[i] != '\0')
		{
			fprintf(stderr, "bzero didn't set a byte that should have been");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	test_memcpy(void)
{
	char		src[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	char		dst[20];
	size_t		bytes;

	bytes = sizeof(src) / sizeof(src[0]);
	printf("Testing Project Libft: ft_memcpy.\n");
	ft_memcpy(dst, src, bytes);
	if (memcmp(dst, src, bytes))
	{
		fprintf(stderr, "memcpy didn't copy a byte that should have been");
		exit(EXIT_FAILURE);
	}
	if (ft_memcpy(NULL, NULL, SIZE_MAX) != NULL)
		fprintf(stderr, RED "memcpy didn't return NULL when both src and dst were NULL." RESET);
}

void	test_memmove(void)
{
	char		buffer[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	char		expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	size_t		bytes;

	bytes = sizeof(buffer) / sizeof(buffer[0]);
	printf("Testing Project Libft: ft_memmove.\n");
	ft_memmove(&buffer[5], buffer, 8);
	if (memcmp(&buffer[5], expected, 8))
	{
		fprintf(stderr, "memmove didn't copy a byte that should have been");
		exit(EXIT_FAILURE);
	}
	if (ft_memmove(NULL, NULL, SIZE_MAX) != NULL)
		fprintf(stderr, RED "memmove didn't return NULL when both src and dst were NULL." RESET);
}

void	test_strlcpy(void)
{
	const t_sctt	test_cases[] = {
		{2, 7, "bonjour", "b"},
		{0, 8, "hihihihi", ""},
	};
	size_t			count;
	size_t			i;
	char			buffer[100];
	size_t			result;
	int				comparison;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_strlcpy.\n");
	while (i < count)
	{
		memset(buffer, 0, sizeof(buffer));
		result = ft_strlcpy(buffer, test_cases[i].src, test_cases[i].size);
		if (result != test_cases[i].expected_return)
		{
			fprintf(stderr, "for test case %zu\n", i);
			fprintf(stderr, "unexpected return value\n");
			exit(EXIT_FAILURE);
		}
		comparison = strcmp(buffer, test_cases[i].expected_dst);
		if (comparison)
		{
			fprintf(stderr, "for test case %zu\n", i);
			fprintf(stderr, "unexpected comparison result: %d\n", comparison);
			fprintf(stderr, "\"%s\" instead of \"%s\"\n", buffer, test_cases[i].expected_dst);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	test_strlcat(void)
{
	const t_scatt	test_cases[] = {
		{0, 4, "0123", "0123456789", "0123456789"},
		{2, 8, "bonjour", "b", "b"},
		{0, 8, "hihihihi", "", ""},
		{0, 4, "abcd", "abcd", "abcd"},
		{4, 4, "", "abcd", "abcd"},
		{12, 4, "", "abcd", "abcd"},
	};
	size_t			count;
	size_t			i;
	char			buffer[100];
	size_t			result;
	int				comparison;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_strlcat.\n");
	while (i < count)
	{
		strlcpy(buffer, test_cases[i].dst, sizeof(buffer));
		result = ft_strlcat(buffer, test_cases[i].src, test_cases[i].size);
		if (result != test_cases[i].expected_return)
		{
			fprintf(stderr, "TC%zu: unexpected return value: %zu instead of %zu\n", i,
				result, test_cases[i].expected_return);
			exit(EXIT_FAILURE);
		}
		comparison = strcmp(buffer, test_cases[i].expected_dst);
		if (comparison)
		{
			fprintf(stderr, "TC%zu: unexpected comparison result: %d\n", i, comparison);
			fprintf(stderr, "\"%s\" instead of \"%s\"\n", buffer, test_cases[i].expected_dst);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	test_strchr(void)
{
	const t_schtc	test_cases[] = {
		{"1234", '1'},
		{"1231", '1'},
		{"4231", '1'},
		{"anticonstitutionellement", ' '},
		{"anticonstitutionellement", 'n'},
		{"test", 'e' + 256},
		{"\0\0", '\0'},
	};
	size_t		count;
	size_t		i;
	char		*libc_result;
	char		*libft_result;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_strchr.\n");
	while (i < count)
	{
		libc_result = strchr(test_cases[i].s1, test_cases[i].c);
		libft_result = ft_strchr(test_cases[i].s1, test_cases[i].c);
		if (libc_result != libft_result)
		{
			fprintf(stderr, "TC%zu: for input \"%s\" and '%c'\n", i, test_cases[i].s1, test_cases[i].c);
			fprintf(stderr, "libc: %p != libft: %p\n", libc_result, libft_result);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	test_strrchr(void)
{
	const t_schtc	test_cases[] = {
		{"1234", '1'},
		{"1231", '1'},
		{"4231", '1'},
		{"anticonstitutionellement", ' '},
		{"anticonstitutionellement", 'n'},
		{"test", 'e' + 256},
		{"\0\0", '\0'},
	};
	size_t		count;
	size_t		i;
	char		*libc_result;
	char		*libft_result;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_strrchr.\n");
	while (i < count)
	{
		libc_result = strrchr(test_cases[i].s1, test_cases[i].c);
		libft_result = ft_strrchr(test_cases[i].s1, test_cases[i].c);
		if (libc_result != libft_result)
		{
			fprintf(stderr, "TC%zu: for input \"%s\" and '%c'\n", i, test_cases[i].s1, test_cases[i].c);
			fprintf(stderr, "libc: %p != libft: %p\n", libc_result, libft_result);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	test_strncmp(void)
{
	const t_scntt	test_cases[] = {
		{2, "bonjour", "bo"},
		{8, "hihihihi", ""},
		{2, "a\377", "ab"},
		{SIZE_MAX, "0", "0"},
	};
	size_t			count;
	size_t			i;
	int				libc_result;
	int				libft_result;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_strncmp.\n");
	while (i < count)
	{
		libc_result = strncmp(test_cases[i].s1, test_cases[i].s2, test_cases[i].size);
		libft_result = ft_strncmp(test_cases[i].s1, test_cases[i].s2, test_cases[i].size);
		if ((libc_result == 0 && libft_result != 0)
			|| (libc_result != 0 && libft_result == 0)
			|| (libc_result < 0 && libft_result >= 0)
			|| (libc_result > 0 && libft_result <= 0))
		{
			fprintf(stderr, "TC%zu: unexpected comparison result: ", i);
			fprintf(stderr, "%d instead of %d\n", libft_result, libc_result);
			fprintf(stderr, "on inputs: \"%s\" and \"%s\"\n", test_cases[i].s1, test_cases[i].s2);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	test_memchr(void)
{
	const t_mchtt	test_cases[] = {
		{"bonjour", 'b', 7},
		{"aaabbbaaa", 'b', 9},
		{"\0\0\0", '\0', 4},
	};
	size_t			count;
	size_t			i;
	char			*libc_result;
	char			*libft_result;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_memchr.\n");
	while (i < count)
	{
		libc_result = memchr(test_cases[i].s, test_cases[i].c, test_cases[i].n);
		libft_result = ft_memchr(test_cases[i].s, test_cases[i].c, test_cases[i].n);
		if (libc_result != libft_result)
		{
			fprintf(stderr, "for test case %zu\n", i);
			fprintf(stderr, "unexpected comparison result: ");
			fprintf(stderr, "%p instead of %p\n", libft_result, libc_result);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	test_memcmp(void)
{
	const t_mctt	test_cases[] = {
		{2, "bonjour", "bo"},
		{8, "hihihihi", "\0\0\0\0\0\0\0"},
	};
	size_t			count;
	size_t			i;
	int				libc_result;
	int				libft_result;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_memcmp.\n");
	while (i < count)
	{
		libc_result = memcmp(test_cases[i].s1, test_cases[i].s2, test_cases[i].size);
		libft_result = ft_memcmp(test_cases[i].s1, test_cases[i].s2, test_cases[i].size);
		if (libc_result != libft_result)
		{
			fprintf(stderr, "for test case %zu\n", i);
			fprintf(stderr, "unexpected comparison result: ");
			fprintf(stderr, "%d instead of %d\n", libft_result, libc_result);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	test_strnstr(void)
{
	const t_snstt	test_cases[] = {
		{"bonjour", "bo", 2},
		{"", "", 0},
		{"", "", SIZE_MAX},
		{"bye", "bye", 0},
		{"hihihihi", "", 8},
		{"hello", "hello", 5},
	};
	size_t			count;
	size_t			i;
	char			*libc_result;
	char			*libft_result;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_strnstr.\n");
	while (i < count)
	{
		libc_result = strnstr(test_cases[i].big, test_cases[i].little, test_cases[i].size);
		libft_result = ft_strnstr(test_cases[i].big, test_cases[i].little, test_cases[i].size);
		if (libc_result != libft_result)
		{
			fprintf(stderr, "for test case %zu\n", i);
			fprintf(stderr, "unexpected comparison result: ");
			fprintf(stderr, "%p instead of %p\n", libft_result, libc_result);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	test_atoi(void)
{
	const char	*test_values[9];
	size_t		count;
	size_t		i;
	int			libc_result;
	int			libft_result;

	test_values[0] = "1234";
	test_values[1] = " \t\n\v\f\r5";
	test_values[2] = "-42";
	test_values[3] = " -+-+-42";
	test_values[4] = "2147483647";
	test_values[5] = "-2147483648";
	test_values[6] = "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
	test_values[7] = "0";
	test_values[8] = "9e";
	count = sizeof(test_values) / sizeof(test_values[0]);
	i = 0;
	printf("Testing Project Libft: ft_atoi.\n");
	while (i < count)
	{
		libc_result = atoi(test_values[i]);
		libft_result = ft_atoi(test_values[i]);
		if (libc_result != libft_result)
		{
			fprintf(stderr, "for input %s", test_values[i]);
			fprintf(stderr, "libc: %d != libft: %d\n", libc_result, libft_result);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	test_calloc(void)
{
	uint8_t	*result;
	size_t	nmemb;
	size_t	size;
	size_t	s;
	size_t	i;

	printf("Testing Project Libft: ft_calloc.\n");
	nmemb = 10;
	size = 10;
	result = ft_calloc(nmemb, size);
	if (result == NULL)
	{
		fprintf(stderr, "returned NULL while it shouldn't have.\n");
		exit(EXIT_FAILURE);
	}
	s = nmemb * size;
	i = 0;
	while (i < s)
	{
		if (result[i] != '\0')
		{
			fprintf(stderr, "memory was not initialized to 0.\n");
			free(result);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	free(result);
	result = ft_calloc(SIZE_MAX, SIZE_MAX);
	if (result != NULL)
	{
		fprintf(stderr, "did not return NULL on overflow.\n");
		free(result);
		exit(EXIT_FAILURE);
	}
}

void	test_strdup(void)
{
	char	*result;
	char	*src = "wonderful!\09";

	printf("Testing Project Libft: ft_strdup.\n");
	result = ft_strdup(src);
	if (result == NULL)
	{
		fprintf(stderr, "ft_strdup has returned NULL while it shouldn't have.\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		if (strcmp(src, result))
		{
			fprintf(stderr, "ft_strdup didn't copy the string correctly.\n");
			exit(EXIT_FAILURE);
		}
		free(result);
	}
}

void	test_substr(void)
{
	const t_sstt	test_cases[] = {
		{1, 2, "bonjour", "on"},
		{UINT_MAX, 2, "bonjour", ""},
		{1, SIZE_MAX, "bonjour le monde", "onjour le monde"},
		{0, 0, "", ""},
	};
	size_t			count;
	size_t			i;
	char			*result;
	int				comparison;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_substr.\n");
	while (i < count)
	{
		result = ft_substr(test_cases[i].src, test_cases[i].start, test_cases[i].len);
		if (result == NULL) {
			fprintf(stderr, "TC%zu: should not have returned NULL.\n", i);
			exit(EXIT_FAILURE);
		}
		comparison = strcmp(result, test_cases[i].result);
		if (comparison)
		{
			fprintf(stderr, "TC%zu: unexpected comparison result: %d", i, comparison);
			free(result);
			exit(EXIT_FAILURE);
		}
		free(result);
		i++;
	}
}

void	test_strjoin(void)
{
	const t_sjtt	test_cases[] = {
		{"bonjour", " le monde", "bonjour le monde"},
		{"", "a", "a"},
		{"b", "", "b"},
		{"", "", ""},
	};
	size_t			count;
	size_t			i;
	char			*result;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_strjoin.\n");
	while (i < count)
	{
		result = ft_strjoin(test_cases[i].s1, test_cases[i].s2);
		if (result == NULL)
		{
			fprintf(stderr, "for test case %zu\n", i);
			fprintf(stderr, "returned NULL");
			exit(EXIT_FAILURE);
		}
		int comparison = strcmp(result, test_cases[i].expected_result);
		if (comparison)
		{
			fprintf(stderr, "for test case %zu\n", i);
			fprintf(stderr, "unexpected comparison result: %d", comparison);
			free(result);
			exit(EXIT_FAILURE);
		}
		free(result);
		i++;
	}
}

void	test_strtrim(void)
{
	const t_sttc	test_cases[] = {
		{"", "\0qqq", ""},
		{"", "", ""},
		{"bonjour", "br", "onjou"},
		{"    \t\v hi", " \t\v", "hi"},
		{"    bonj   our    ", "  ", "bonj   our"},
		{"                     ", " ", ""},
		{"abcdba", "acb", "d"},
	};
	size_t			count;
	size_t			i;
	char			*result;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_strtrim.\n");
	while (i < count)
	{
		result = ft_strtrim(test_cases[i].s1, test_cases[i].set);
		if (result == NULL)
		{
			fprintf(stderr, "TC%zu: returned NULL\n", i);
			exit(EXIT_FAILURE);
		}
		int comparison = strcmp(result, test_cases[i].expected_result);
		if (comparison)
		{
			fprintf(stderr, "TC%zu: unexpected comparison result: %d\n", i, comparison);
			fprintf(stderr, "got \"%s\" instead of \"%s\"\n", result, test_cases[i].expected_result);
			free(result);
			exit(EXIT_FAILURE);
		}
		free(result);
		i++;
	}
}

void	test_split(void)
{
	const t_spltc	test_cases[] = {
		{"b o n", ' ', {"b", "o", "n", NULL}},
		{"   b   o   n   ", ' ', {"b", "o", "n", NULL}},
		{"bonjour", ' ', {"bonjour", NULL, NULL, NULL}},
		{"hello world", '\0', {"hello world", NULL, NULL, NULL}},
	};
	size_t			count;
	size_t			i;
	size_t			j;
	char			**result;
	int				comparison;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_split.\n");
	while (i < count)
	{
		result = ft_split(test_cases[i].s, test_cases[i].c);
		if (result == NULL)
		{
			fprintf(stderr, "TC%zu: returned NULL\n", i);
			exit(EXIT_FAILURE);
		}
		j = 0;
		while (test_cases[i].expected[j])
		{
			if (result[j] == NULL)
			{
				fprintf(stderr, "TC%zu: unexpected NULL at position %zu\n", i, j);
				exit(EXIT_FAILURE);
			}
			comparison = strcmp(result[j], test_cases[i].expected[j]);
			if (comparison)
			{
				fprintf(stderr, "TC%zu: unexpected comparison result: %d\n", i, comparison);
				fprintf(stderr, "got \"%s\" instead of \"%s\"\n", result[j], test_cases[i].expected[j]);
				exit(EXIT_FAILURE);
			}
			free(result[j]);
			j++;
		}
		free(result);
		i++;
	}
}

void	test_itoa(void)
{
	const t_iatc	test_cases[] = {
		{0, "0"},
		{42, "42"},
		{10, "10"},
		{-10, "-10"},
		{106, "106"},
		{INT_MIN, "-2147483648"},
		{INT_MAX, "2147483647"},
	};
	size_t			count;
	size_t			i;
	char			*result;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_itoa.\n");
	while (i < count)
	{
		result = ft_itoa(test_cases[i].n);
		if (result == NULL)
		{
			fprintf(stderr, "TC%zu: returned NULL\n", i);
			exit(EXIT_FAILURE);
		}
		int comparison = strcmp(result, test_cases[i].expected_result);
		if (comparison)
		{
			fprintf(stderr, "TC%zu: unexpected comparison result: %d\n", i,
				comparison);
			fprintf(stderr, "got \"%s\" instead of \"%s\"\n", result,
				test_cases[i].expected_result);
			free(result);
			exit(EXIT_FAILURE);
		}
		free(result);
		i++;
	}
}

char	strmapi_toupper(unsigned int i, char c)
{
	(void)i;
	return (toupper(c));
}

char	strmapi_tolower(unsigned int i, char c)
{
	(void)i;
	return (tolower(c));
}

void	test_strmapi(void)
{
	const t_smatc	test_cases[] = {
		{"bonJour", strmapi_toupper, "BONJOUR"},
		{"BONjOUR", strmapi_tolower, "bonjour"},
	};
	size_t			count;
	size_t			i;
	char			*result;
	int				comparison;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_strmapi.\n");
	while (i < count)
	{
		result = ft_strmapi(test_cases[i].src, test_cases[i].f);
		if (result == NULL)
		{
			fprintf(stderr, "TC%zu: returned NULL\n", i);
			exit(EXIT_FAILURE);
		}
		comparison = strcmp(result, test_cases[i].expected_result);
		if (comparison)
		{
			fprintf(stderr, "TC%zu: unexpected comparison result: %d\n",
				i, comparison);
			fprintf(stderr, "got \"%s\" instead of \"%s\"\n", result,
				test_cases[i].expected_result);
			free(result);
			exit(EXIT_FAILURE);
		}
		free(result);
		i++;
	}
}

void	striteri_toupper(unsigned int i, char *c)
{
	(void)i;
	*c = toupper(*c);
}

void	striteri_tolower(unsigned int i, char *c)
{
	(void)i;
	*c = tolower(*c);
}

void	striteri_index(unsigned int i, char *c)
{
	*c = i % 10 + '0';
}

void	test_striteri(void)
{
	const t_siitc	test_cases[] = {
		{"bonJour", striteri_toupper, "BONJOUR"},
		{"BONjOUR", striteri_tolower, "bonjour"},
		{"9876543", striteri_index, "0123456"},
	};
	size_t			count;
	size_t			i;
	char			*result;
	int				comparison;

	count = sizeof(test_cases) / sizeof(test_cases[0]);
	i = 0;
	printf("Testing Project Libft: ft_striteri.\n");
	while (i < count)
	{
		result = strdup(test_cases[i].src);
		if (result == NULL)
		{
			fprintf(stderr, "TC%zu: internal allocation failure\n", i);
			exit(EXIT_FAILURE);
		}
		ft_striteri(result, test_cases[i].f);
		comparison = strcmp(result, test_cases[i].expected_result);
		if (comparison)
		{
			fprintf(stderr, "TC%zu: unexpected comparison result: %d\n", i, comparison);
			fprintf(stderr, "got \"%s\" instead of \"%s\"\n", result, test_cases[i].expected_result);
			free(result);
			exit(EXIT_FAILURE);
		}
		free(result);
		i++;
	}
}

void	test_putchar_fd(void)
{
	int		test_pipe[2];
	char	buf[100];
	ssize_t	read_ret;
	char	test_c;

	printf("Testing Project Libft: ft_putchar_fd.\n");
	test_c = '1';
	if (pipe(test_pipe))
	{
		fprintf(stderr, "Pipe failed.\n");
		exit(EXIT_FAILURE);
	}
	ft_putchar_fd(test_c, test_pipe[1]);
	close(test_pipe[1]);
	read_ret = read(test_pipe[0], &buf, sizeof(buf) / sizeof(buf[0]));
	if (read_ret != 1)
	{
		close(test_pipe[0]);
		fprintf(stderr, "Read failed with ret %zd.\n", read_ret);
		exit(EXIT_FAILURE);
	}
	close(test_pipe[0]);
	if (buf[0] != test_c)
	{
		fprintf(stderr, "Invalid character written: %c instead of %c.\n", buf[0], test_c);
		exit(EXIT_FAILURE);
	}
}

void	test_putstr_fd(void)
{
	int		test_pipe[2];
	ssize_t	read_ret;
	char	buf[100];
	char	*str;
	size_t	str_len;

	printf("Testing Project Libft: ft_putstr_fd.\n");
	str = "bonjour";
	str_len = ft_strlen(str);
	if (pipe(test_pipe))
	{
		fprintf(stderr, "Pipe failed.\n");
		exit(EXIT_FAILURE);
	}
	ft_putstr_fd(str, test_pipe[1]);
	close(test_pipe[1]);
	read_ret = read(test_pipe[0], buf, sizeof(buf) / sizeof(buf[0]));
	close(test_pipe[0]);
	if (read_ret != (ssize_t) str_len)
	{
		fprintf(stderr, "Read failed with ret %zd.\n", read_ret);
		exit(EXIT_FAILURE);
	}
	if (strncmp(buf, str, str_len))
	{
		fprintf(stderr, "Invalid string written.\n");
		exit(EXIT_FAILURE);
	}
}

void	test_putendl_fd(void)
{
	int		test_pipe[2];
	ssize_t	read_ret;
	char	buf[100];
	char	*str;
	char	*expected;
	size_t	str_len;

	printf("Testing Project Libft: ft_putendl_fd.\n");
	str = "bonjour";
	expected = "bonjour\n";
	str_len = ft_strlen(str);
	if (pipe(test_pipe))
	{
		fprintf(stderr, "Pipe failed.\n");
		exit(EXIT_FAILURE);
	}
	ft_putendl_fd(str, test_pipe[1]);
	close(test_pipe[1]);
	read_ret = read(test_pipe[0], buf, sizeof(buf) / sizeof(buf[0]));
	close(test_pipe[0]);
	if (read_ret != (ssize_t) str_len + 1)
	{
		fprintf(stderr, "Read failed with ret %zd.\n", read_ret);
		exit(EXIT_FAILURE);
	}
	if (strncmp(buf, expected, str_len + 1))
	{
		fprintf(stderr, "Invalid string written.\n");
		exit(EXIT_FAILURE);
	}
}

void	test_putnbr_fd(void)
{
	int		test_pipe[2];
	ssize_t	read_ret;
	char	buf[100];
	size_t	i;
	t_pntt	test_cases[] = {
		{1234, "1234"},
		{-42, "-42"},
		{0, "0"},
		{10, "10"},
		{106, "106"},
		{INT_MIN, "-2147483648"},
		{INT_MAX, "2147483647"},
	};

	i = 0;
	printf("Testing Project Libft: ft_putnbr_fd.\n");
	while (i < sizeof(test_cases) / sizeof(test_cases[0]))
	{
		if (pipe(test_pipe))
		{
			fprintf(stderr, "Pipe failed.\n");
			exit(EXIT_FAILURE);
		}
		ft_putnbr_fd(test_cases[i].input, test_pipe[1]);
		close(test_pipe[1]);
		read_ret = read(test_pipe[0], buf, sizeof(buf) / sizeof(buf[0]));
		close(test_pipe[0]);
		if (read_ret != (ssize_t) ft_strlen(test_cases[i].expected))
		{
			fprintf(stderr, "TC%zu: read failed with ret %zd.\n", i, read_ret);
			exit(EXIT_FAILURE);
		}
		if (strncmp(buf, test_cases[i].expected, ft_strlen(test_cases[i].expected)))
		{
			fprintf(stderr, "TC%zu: invalid string written.\n", i);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

int	main(void)
{
	printf("Testing Project Libft.\n");
	test_chrclass(isalpha, ft_isalpha, "ft_isalpha");
	test_chrclass(isdigit, ft_isdigit, "ft_isdigit");
	test_chrclass(isalnum, ft_isalnum, "ft_isalnum");
	test_chrclass(isascii, ft_isascii, "ft_isascii");
	test_chrclass(isprint, ft_isprint, "ft_isprint");
	test_strlen();
	test_memset();
	test_bzero();
	test_memcpy();
	test_memmove();
	test_strlcpy();
	test_strlcat();
	test_chrclass(toupper, ft_toupper, "ft_toupper");
	test_chrclass(tolower, ft_tolower, "ft_tolower");
	test_strchr();
	test_strrchr();
	test_strncmp();
	test_memchr();
	test_memcmp();
	test_strnstr();
	test_atoi();
	test_calloc();
	test_strdup();
	test_substr();
	test_strjoin();
	test_strtrim();
	test_split();
	test_itoa();
	test_strmapi();
	test_striteri();
	test_putchar_fd();
	test_putstr_fd();
	test_putendl_fd();
	test_putnbr_fd();
#ifdef BONUS
	test_lstnew();
	test_lstadd_front();
	test_lstsize();
	test_lstlast();
	test_lstadd_back();
	test_lstdelone();
	test_lstclear();
	test_lstiter();
	test_lstmap();
#endif
	printf(GREEN "You passed all required tests! Congrats!\n" RESET);
}
