#!/usr/bin/env bash
# Find cwd for all login shells

grep=/usr/bin/grep
me=$PPID
for x in `ps -ef | $grep "\-bash" | $grep -v grep | awk '{print $2}' | sort -u`
do
  if [ $x -eq $me ]
  then
    msg="*"
  else
    msg=" "
  fi

  echo "$msg $x: " `ls -al /proc/$x/ | $grep '^l' | $grep cwd |  awk '{print $3, $11}'`
done
