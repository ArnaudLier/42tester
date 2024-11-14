#!/bin/sh

IN=in
OUT=out

truncate -s 0 "$IN"
rm "$OUT"

echo "bonjour1234\na1 a a" > $IN

# < $IN cat | tr '[:lower:]' '[:upper:]' > $OUT
./pipex $IN 'cat' "tr '[:lower:]' '[:upper:]'" $OUT

RESULT=$(cat "$OUT")
if [ "$RESULT" != "BONJOUR1234
A1 A A" ]; then
	echo "TC1: failed, got $RESULT"
fi

./pipex $IN "ls -l" "wc -l" $OUT

RESULT=$(cat "$OUT")
SH_RESULT=$(ls -l | wc -l)
if [ "$RESULT" != "$SH_RESULT" ]; then
	echo "TC2: failed, got $RESULT"
fi

./pipex $IN "grep a1" "wc -w" $OUT
RESULT=$(cat "$OUT")
if [ "$RESULT" != "3" ]; then
	echo "TC3: failed, got $RESULT"
fi

#### BONUS #####

# < $IN cat | tr '[:lower:]' '[:upper:]' | tr -d '[:digit:]' > $OUT
./pipex $IN 'cat' "tr '[:lower:]' '[:upper:]'" "tr -d '[:digit:]'" $OUT

RESULT=$(cat "$OUT")
if [ "$RESULT" != "BONJOUR
A A A" ]; then
	echo "bonus: TC1: failed, got $RESULT"
fi

truncate -s 0 "$OUT"

# cat << EOF | tr '[:lower:]' '[:upper:]' >> $OUT
echo "hi\nEOF" | ./pipex here_doc EOF cat "tr '[:lower:]' '[:upper:]'" $OUT

RESULT=$(cat "$OUT")
if [ "$RESULT" != "HI" ]; then
	echo "bonus: TC2: failed"
fi