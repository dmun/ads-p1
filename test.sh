#!/bin/bash

make compile

for input in samples-Practice/*.in; do
    ans="${input%.in}.ans"
    testname=$(basename "$input" .in)

    echo -n "test $testname: "

    if ./main < "$input" | diff -q - "$ans" > /dev/null 2>&1; then
        echo "✓ PASSED"
    else
        echo "✗ FAILED"
        ./main < "$input" | diff - "$ans" --color
        echo
    fi
done
