#~/usr/bin/env bash

if [ $# -ne 1 ]; then
  echo "Usage: `basename $0` <number-of-columns>"
  exit 1
fi

N=$1
sql="create table t(c0 int primary key"
for x in `seq 1 $N`; do
  sql="$sql, c$x mediumblob"
done
sql="$sql) engine = innodb;"
echo $sql

sql="insert into t values(1"
for x in `seq 1 $N`; do
  sql="$sql, repeat('x',7680)"
done
sql="$sql);"
echo $sql

