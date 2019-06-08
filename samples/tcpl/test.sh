#!/bin/sh

prog=$1

. ./setenv.sh $prog

if [ -f "$prog.sh" ]
then
	./$prog.sh $prog
else
	./$prog
fi

