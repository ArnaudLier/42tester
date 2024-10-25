#!/bin/sh

make
./main | xxd > libft_output
./main libc | xxd > libc_output
DIFF_OUTPUT=$(diff libc_output libft_output)

if [ -z "$DIFF_OUTPUT" ]; then
	echo "Tests passed! Congrats!"
else
	echo "No cookie for you today :("
	echo "$DIFF_OUTPUT"
fi
