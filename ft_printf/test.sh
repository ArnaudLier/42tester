#!/bin/sh

make
./main > libft_output
./main libc > libc_output
DIFF_OUTPUT=$(diff -a libc_output libft_output)

if [ -z "$DIFF_OUTPUT" ]; then
	echo "\e[1;32mTests passed! Congrats!\e[0m"
else
	echo "\e[1;31mNo cookie for you today :(\e[0m"
	echo "< libc"
	echo "> libft"
	echo "$DIFF_OUTPUT"
fi
