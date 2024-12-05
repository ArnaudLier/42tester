#!/bin/bash

test() {
	echo "BUFFER_SIZE = $1"
	make -s re BUFFER_SIZE=$1
	diff <(./mandatory <input.txt) expected_output
	diff <(./bonus <input.txt) expected_output_bonus
}

test 1
test 10
test 1000000 
