#!/bin/bash

./main | xxd > libft_output
./main libc | xxd > libc_output
diff libc_output libft_output
