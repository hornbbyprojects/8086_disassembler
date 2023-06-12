#!/bin/bash -e


if [ -z $1 ]
then
    echo "Usage: $0 [[binary filepath]]"
    exit 1
fi

directory=$(mktemp -d)
asm_path=$directory/test.asm
binary_path=$directory/test
./disassembler $1 > $asm_path
nasm $asm_path > $binary_path
if cmp "$1" "$binary_path"
then
    echo "Files matched!"
else
    echo "Files differed!"
fi


