#!/usr/bin/env bash

prog=./template

if [ $# -eq 2 ]; then
    start=$1
    end=$2
else
    start=1
    end=2
fi

echo "Running tests from $start to $end ..."
for x in $(seq $start $end); do
    echo "test case: $x"
    in=input$x.txt
    out=output$x.txt
    exp=expect$x.txt
    # prog, in, out, exp should exist
    $prog < $in > $out
    diff $out $exp
done
