#!/bin/sh

check()
{
	total=`expr $total + 1`
	echo exp: "$1"
	echo res: "$2"

	if [ "$1" = "$2" ]
	then
		passed=`expr $passed + 1`
		echo .
	else
		failed=`expr $failed + 1`
		echo F
	fi
}

begin()
{
	echo Testing exercise "$1"...
}

summary()
{
	echo
	echo Total: $total, Passed: $passed, Failed: $failed.
}

my=`basename $0`
prog=$1
total=0
passed=0
failed=0

if [ x"$prog" = x"" ]
then
	echo No progname.
	exit 1
fi

begin "$prog"

