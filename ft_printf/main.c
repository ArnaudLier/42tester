/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alier <alier@student.42mulhouse.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:22:01 by alier             #+#    #+#             */
/*   Updated: 2024/11/07 17:12:09 by alier            ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	int	(*printfn)(const char *restrict, ...);
	int	bytes_written;

	printfn = ft_printf;
	bytes_written = 0;
	if (argc >= 2 && strcmp(argv[1], "libc") == 0)
		printfn = printf;
	printfn("Conversion Specification\tResult\n");

	/* CHAR */
	TEST_PRINT("%c", 'c');
	TEST_PRINT("%c", 0);
#ifdef BONUS
	TEST_PRINT("%1c", 'c');
	TEST_PRINT("%2c", 'c');
	TEST_PRINT("%3c", 'c');
# ifdef UB
	TEST_PRINT("%.c", 'a');
# endif
#endif

	/* STRING */
	TEST_PRINT("%s", "bonjour");
	TEST_PRINT("%s", "42");
	TEST_PRINT("%s", "0");
	TEST_PRINT("%s", "");
#ifdef UB
	TEST_PRINT("%s", NULL);
#endif
#ifdef BONUS
	TEST_PRINT("%.0s", "bonjour");
	TEST_PRINT("%.5s", "bonjour");
	TEST_PRINT("%.13s", "bonjour");
	TEST_PRINT("%50.13s", "bonjour");
	TEST_PRINT("%-50.13s", "bonjour");
# ifdef UB
	TEST_PRINT("%3.1s", NULL);
	TEST_PRINT("%.2s", NULL);
	TEST_PRINT("%.5s", NULL);
	TEST_PRINT("%.6s", NULL);
	TEST_PRINT("%.50s", NULL);
# endif
#endif

	/* POINTER */
	TEST_PRINT("%p", main);
#ifdef BONUS
	TEST_PRINT("%-20p", main);
	TEST_PRINT("%20p", main);
#endif
#ifdef UB
	TEST_PRINT("%p", NULL);
#endif

	/* DECIMAL */
	TEST_PRINT("%i", 0);
	TEST_PRINT("%i", 42);
	TEST_PRINT("d %i a", 42);
	TEST_PRINT("%i", INT_MAX);
	TEST_PRINT("%i", INT_MIN);
	TEST_PRINT("%d", 0);
	TEST_PRINT("%d", 42);
	TEST_PRINT("a %d b", 42);
	TEST_PRINT("%d", INT_MAX);
	TEST_PRINT("%d", INT_MIN);
#ifdef BONUS
	TEST_PRINT("%04d", -16);
	TEST_PRINT("% 4d", -16);
	TEST_PRINT("%.d", 0);
	TEST_PRINT("% d", 0);
	TEST_PRINT("%+4d", 16);
	TEST_PRINT("%+10.5d", -50);
	TEST_PRINT("%+ 10.5d", -50);
	TEST_PRINT("%+10.5d", 16);
	TEST_PRINT("%+.3d", 16);
	TEST_PRINT("%+d", 16);
	TEST_PRINT("%+d", 0);
	TEST_PRINT("%+- 10.5d", 4);
	TEST_PRINT("%+-  10.5d", -4);
	TEST_PRINT("%0-10.5d", 4);
	TEST_PRINT("% .5d", 4);
	TEST_PRINT("%10.5d", 32);
	TEST_PRINT("%3.5d", 24);
	TEST_PRINT("%06.5d", 24);
	TEST_PRINT("%.5d", -54);
	TEST_PRINT("%05.d", 999);
	TEST_PRINT("%003d", 42);
	TEST_PRINT("%.010d", 42);
#endif

	/* UNSIGNED */
	TEST_PRINT("%u", 0);
	TEST_PRINT("%u", 124);
	TEST_PRINT("%u", UINT_MAX);
#ifdef BONUS
	TEST_PRINT("%05.2u", 26);
	TEST_PRINT("%.5u", 0);
	TEST_PRINT("%30u", UINT_MAX);
	TEST_PRINT("%-30u", UINT_MAX);
#endif

	/* HEXADECIMAL */
	TEST_PRINT("%x", 42);
	TEST_PRINT("%x", UINT_MAX);
	TEST_PRINT("%x", 0);
	TEST_PRINT("%X", "bonjour", 42);
	TEST_PRINT("%X", 0);
#ifdef BONUS
	TEST_PRINT("%15.5X", 4);
	TEST_PRINT("%3.5X", 4);
	TEST_PRINT("%.5X", -54);
	TEST_PRINT("%015x", 0);
	TEST_PRINT("%015X", 0);
	TEST_PRINT("%0-15X", 0);
	TEST_PRINT("%-5.x", 0);
	TEST_PRINT("%-5.X", 0);
	TEST_PRINT("%#15.5X", 987);
	TEST_PRINT("%#x", 0);
	TEST_PRINT("%#X", 0);
	TEST_PRINT("%#x", 42);
	TEST_PRINT("%0#20x", 42);
	TEST_PRINT("%#X", 42);
#endif

	/* ESCAPE */
	TEST_PRINT("%%", 0);
#ifdef UB
	TEST_PRINT("%", 0);
	TEST_PRINT("%z", 0);
	TEST_PRINT("%000z", 0);
	TEST_PRINT("%3%", 0);
#endif

	/* MISC */
	TEST_PRINT("Bonjour %ce monde ! %d is your answer.", 'l', 42);

	TEST_PRINT("Bytes written: %d", bytes_written);
	return (EXIT_SUCCESS);
}
