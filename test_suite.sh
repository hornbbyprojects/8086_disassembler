#!/bin/bash -e
to_test_disassemble=(
    "listing_0037_single_register_mov"
    "listing_0038_many_register_mov"
    "listing_0039_more_movs"
    "listing_0040_challenge_movs"
    "listing_0041_add_sub_cmp_jnz")


for testfile in ${to_test_disassemble[@]}; do
    echo "Testing dissassembler on $testfile"
    ./test_disassemble.sh asms/$testfile
done

to_test_simulate=(
    "listing_0043_immediate_movs"
    "listing_0044_register_movs"
    "listing_0046_add_sub_cmp"
    "listing_0049_conditional_jumps"
    "listing_0051_memory_mov"
    "listing_0052_memory_add_loop")



for testfile in ${to_test_simulate[@]}; do
    echo "Testing simulation on $testfile"
    ./test_simulate.sh asms/$testfile
done
