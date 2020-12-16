#!/usr/bin/env bash

if [ $# != 1 ]; then
  echo "Usage: `basename $0` <colorfulFile>"
  exit 1
fi

file="$1"
if [ ! -f "$file" ]; then
  echo "ERROR: $file not exists or not a file."
  exit 1
fi

sed -e 's/\x1B\[[0-9A-Za-z]\{0,2\}\x1B\[K//g' \
    -e 's/\x1B\[K//g' \
    -e 's/\x1B\[[0-9]m//g' \
    -e 's/\x1B\[[0-9][0-9]m//g' \
    -e 's/\x1B\[[0-9][0-9];[0-9][0-9]m//g' \
    "$file"
