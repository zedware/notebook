#!/usr/bin/env bash

usage() {
  echo "`basename $0` <N> (jenkins|wsl|dev)"
  echo "  <N> is the number of sessions wants to create"
  echo "  jenkins is the alias of the host defined in /etc/hosts"
  echo ""
  exit 1
}

is_number() {
  re='^[0-9]+$'
  if ! [[ $1 =~ $re ]] ; then
    echo "error: Argument \"$1\" is not a number" >&2
    exit 1
  fi
}

# Let the first window ssh to remote, too.
# This will add to the number of windows created, thus make actual 
# windows to be N + 1.
modify_current_window=0

if [ $# -eq 2 ];
then
  N=$1
  V=$2
else
  usage 
fi

is_number $N

if [ x"$V" = x"jenkins" -o x"$V" = x"wsl" -o x"$V" = x"dev" ]
then
  ssh_vm="ssh $V"
else
  usage
fi

# Change the first window's name to $prefix-0
prefix=$V
if [ x"$modify_current_window" = x"1" ]; then
  tmux rename-window "$prefix-0"
fi

initial=`tmux list-panes -s -F#{window_name} | grep "^$prefix" | \
  awk -F- '{print $2}' | grep '^[0-9]\{1\}$' | sort -nr | head -1`
if [ x"$initial" = x"" ];
then
  initial=0
fi

for x in `seq 1 $N`;
do
  n=$(($initial + $x))
  tmux new-window -d -n "$prefix-$n" "$ssh_vm"
done

if [ x"$modify_current_window" = x"1" ]; then
  $ssh_vm
fi
