#!/bin/sh

make re BUFFER_SIZE=1
./mandatory
./bonus
make re BUFFER_SIZE=10
./mandatory
./bonus
make re BUFFER_SIZE=1000000
./mandatory
./bonus
