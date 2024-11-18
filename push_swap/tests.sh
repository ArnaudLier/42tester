#!/bin/bash

CHECKER=./checker_linux
#CHECKER=./checker

RESET="\e[0m"
RED="\e[1;31m"
GREEN="\e[1;32m"

test()
{
	NUMBER_COUNT=$1
	TEST_COUNT=$2
	OBJECTIVE=$3
	echo "Testing $NUMBER_COUNT random numbers $TEST_COUNT time(s)..."
	TOTAL=0
	MINIMUM=999999999999999999
	MAXIMUM=0
	for _ in $(seq "$TEST_COUNT")
	do
		ARG=$(shuf -i 0-4294967295 -n "$NUMBER_COUNT" | awk '{for(i=1;i<=NF;i++){$i=$i-2147483648}}1')
		#echo "Trying $(echo "$ARG" | tr '\n' ' ')"
		IFS= read -rd '' INSTRUCTIONS < <( ./push_swap $ARG )
		#echo "Instructions: $INSTRUCTIONS"
		INSTRUCTION_COUNT=$(echo -n "$INSTRUCTIONS" | grep -c '^')
		if [ "$MINIMUM" -gt "$INSTRUCTION_COUNT" ]; then
			MINIMUM=$INSTRUCTION_COUNT
		fi
		if [ "$MAXIMUM" -lt "$INSTRUCTION_COUNT" ]; then
			MAXIMUM=$INSTRUCTION_COUNT
		fi
		TOTAL=$((TOTAL + INSTRUCTION_COUNT))
		RESULT=$(echo -n "$INSTRUCTIONS" | $CHECKER $ARG 2>&1)
		if [ "$RESULT" != "OK" ]; then
			echo "KO on input $(echo "$ARG" | tr '\n' ' '): "$RESULT""
			exit 1
		fi
	done
	echo "Minimum: $MINIMUM instructions"
	echo "Average: $((TOTAL/TEST_COUNT)) instructions"
	echo "Maximum: $MAXIMUM instructions"
	if [ "$MAXIMUM" -le "$OBJECTIVE" ]; then
		echo -ne $GREEN
	else
		echo -ne $RED
	fi
	echo -e "Objective: <= $OBJECTIVE instructions"$RESET
	echo ""
}

test 1 1 0
test 2 10 1
test 3 300 3
test 5 300 12
test 100 100 700
test 500 100 5500

if [ "$(./push_swap 1 1 2>&1 >/dev/null)" != "Error" ]; then
	echo -e $RED"does not error on duplicates"$RESET
fi

if [ "$(./push_swap 1 1e 2>&1 >/dev/null)" != "Error" ]; then
	echo -e $RED"does not error on invalid number"$RESET
fi

if [ "$(./push_swap 1 one 2>&1 >/dev/null)" != "Error" ]; then
	echo -e $RED"does not error on invalid number"$RESET
fi

if [ "$(./push_swap 2147483648 2>&1 >/dev/null)" != "Error" ]; then
	echo -e $RED"does not error on overflow"$RESET
fi

if [ "$(./push_swap 99999999999999 2>&1 >/dev/null)" != "Error" ]; then
	echo -e $RED"does not error on overflow"$RESET
fi

if [ "$(./push_swap -2147483649 2>&1 >/dev/null)" != "Error" ]; then
	echo -e $RED"does not error on underflow"$RESET
fi

if [ "$(./push_swap -21474836490000 2>&1 >/dev/null)" != "Error" ]; then
	echo -e $RED"does not error on underflow"$RESET
fi

# Checker tests
RESULT=$(echo -n "sa" | $CHECKER 2 1 2>&1 >/dev/null)
if [ "$RESULT" != "Error" ]; then
	echo -e $RED"does not error on missing newline"$RESET
fi