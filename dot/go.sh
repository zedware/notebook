#!/usr/bin/env bash

for x in `ls -1 *.dot`; do
  echo $x
  dot -T png -o `basename -s .dot $x`.png $x
done

