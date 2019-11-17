#!/bin/bash

usage()
{
  echo "mysql.sh (m55|s55|m56|s56) [<user>]"
  echo ""
  exit 1
}

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

if [ x"$2" = x"" ]
then
  user="mysql"
else
  user="$2"
fi

homedir=/u01/$dir
cd $homedir/bin
./mysql -u $user -S $homedir/run/mysql.sock test
