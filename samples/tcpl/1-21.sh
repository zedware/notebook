#!/bin/sh

prog=$1

. ./setenv.sh $prog

prog2=./$prog

exp=""
res=`echo "" | $prog2`
check "$exp" "$res"

exp=" "
res=`echo " " | $prog2`
check "$exp" "$res"

exp=`printf "\t"`
res=`echo "        " | $prog2`
check "$exp" "$res"

exp=`printf "\t"`
res=`echo $exp | $prog2`
check "$exp" "$res"

exp="x y"
res=`echo "x y" | $prog2`
check "$exp" "$res"

exp="x\ty"
res=`echo "x\ty" | $prog2`
check "$exp" "$res"

exp="x  \ty"
res=`echo "x  \ty" | $prog2`
check "$exp" "$res"

exp="x\t "
res=`echo "x\t " | $prog2`
check "$exp" "$res"

exp="\n"
res=`echo "\n" | $prog2`
check "$exp" "$res"

summary 

