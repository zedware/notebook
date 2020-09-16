#!/usr/bin/env bash

if [ $# != 1 ]; then
  echo "Usage: `basename $0` <colorfulFile>"
  exit 1
fi

file="$1"
if [ ! -f "$file" ]; then
  echo "ERROR: $file not exists or not a file.
  exit 1
fi

# \x1B is ^[, \[ is [.
sed -i -e 's/\x1B\[[0-9A-Za-z]\{1,\}\x1B\[K//g' -e 's/\x1B\[K//g' "$file"
