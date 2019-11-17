#!/bin/bash

usage()
{
  echo "mysqld.sh (m55|s55|m56|s56)"
  echo ""
  exit 1
}

if [ $# -gt 1 ]
then
  usage
fi

if [ x"$1" = x"m55" ]
then
  dir=my4006
elif [ x"$1" = x"s55" ]
then
  dir=my4106
elif [ x"$1" = x"m56" ]
then
  dir=my5006
elif [ x"$1" = x"s56" ]
then
  dir=my5106
else
  usage
fi

homedir=/u01/$dir
cd $homedir/bin
echo "Starting MySQL at $homedir..."
nohup ./mysqld --defaults-file=$homedir/my.cnf & 
