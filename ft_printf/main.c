/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alier <alier@student.42mulhouse.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:22:01 by alier             #+#    #+#             */
/*   Updated: 2024/10/22 22:19:43 by alier            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include "ft_printf.h"

#define TEST_PRINT(fmt, ...) bytes_written += printfn("%s\t\t\t\t|" fmt "|\n", fmt, __VA_ARGS__)
#define BUF_SIZE 4242

int	main(int argc, char **argv)
{
	int	(*printfn)(const char *restrict, ...);
	int	bytes_written;

	printfn = ft_printf;
	bytes_written = 0;
	if (argc >= 2 && strcmp(argv[1], "libc") == 0)
		printfn = printf;
	/* CHAR */
	printfn("Conversion Specification\tResult\n");
	TEST_PRINT("%c", 'c');
	TEST_PRINT("%1c", 'c');
	TEST_PRINT("%2c", 'c');
	TEST_PRINT("%3c", 'c');

	/* STRING */
	TEST_PRINT("%s", "bonjour");
	TEST_PRINT("%.5s", "bonjour");
	TEST_PRINT("%.13s", "bonjour");
	TEST_PRINT("%50.13s", "bonjour");
	TEST_PRINT("%-50.13s", "bonjour");

	/* POINTER */
	TEST_PRINT("%p", main);
	// TEST_PRINT("%p", NULL);	[UB]

	/* DECIMAL */
	TEST_PRINT("%i", 42);
	TEST_PRINT("%i", INT_MAX);
	TEST_PRINT("%i", INT_MIN);
	TEST_PRINT("%3.5d", 4);
	TEST_PRINT("%.5d", -54);
	TEST_PRINT("%d", 42);
	TEST_PRINT("%003d", 42);
	TEST_PRINT("%dd", 42);
	TEST_PRINT("%.010d", 42);
	TEST_PRINT("%d", 0);

	/* UNSIGNED */
	TEST_PRINT("%u", 0);
	TEST_PRINT("%u", 124);
	TEST_PRINT("%u", UINT_MAX);
	TEST_PRINT("%30u", UINT_MAX);
	TEST_PRINT("%-30u", UINT_MAX);

	/* HEXADECIMAL */
	TEST_PRINT("%x", 42);
	TEST_PRINT("%x", INT_MAX);
	TEST_PRINT("%x", INT_MIN);
	TEST_PRINT("%3.5X", 4);
	TEST_PRINT("%.5X", -54);
	TEST_PRINT("%X", "bonjour", 42);
	TEST_PRINT("%X", 0);
	TEST_PRINT("%015x", 0);
	TEST_PRINT("%015X", 0);
	TEST_PRINT("%0-15X", 0);
	TEST_PRINT("%#x", 0);
	TEST_PRINT("%#X", 0);
	TEST_PRINT("%#x", 42);
	TEST_PRINT("%#X", 42);

	/* ESCAPE */
	// TEST_PRINT("%", 0);		[UB]
	// TEST_PRINT("%z", 0);		[UB]
	// TEST_PRINT("%000z", 0);	[UB]
	TEST_PRINT("%%", 0);
	// TEST_PRINT("%3%", 0);	[UB]

	TEST_PRINT("Bytes written: %d", bytes_written);
	return (EXIT_SUCCESS);
}
