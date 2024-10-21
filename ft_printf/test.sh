#!/bin/bash

./main > libft_output
./main libc > libc_output
diff libc_output libft_output
