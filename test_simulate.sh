#!/bin/bash -e


if [ -z $1 ]
then
    echo "Usage: $0 [[binary filepath]]"
    exit 1
fi

directory=$(mktemp -d)
actual_output=$directory/actual_output.txt
expected_output=$directory/expected_output.txt
./disassembler simulate $1 | sed -n '/Final state:/,$p' | sed '/flags:/,$d'| sed '/(0)/d' | tail -n +2  > $actual_output
sed -n '/Final registers:/,$p' $1.txt | sed '/flags/d' | sed '/ip/d' | tail -n +2 > $expected_output

if diff -Bw $actual_output $expected_output > /dev/null
then
    echo "Files matched!"
else
    echo "Files differed!"
    diff -Bw $actual_output $expected_output
fi
