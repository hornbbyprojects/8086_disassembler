#!/bin/bash -e
to_test=(
    "listing_0037_single_register_mov"
    "listing_0038_many_register_mov"
    "listing_0039_more_movs"
    "listing_0040_challenge_movs"
    "listing_0041_add_sub_cmp_jnz")


for testfile in ${to_test[@]}; do
    echo "Testing $testfile"
    ./test.sh asms/$testfile
done
