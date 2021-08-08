#!/usr/bin/env bash

if [ $# != 1 ]; then
  echo "Usage: `basename $0` <newProgramName>"
  echo "       Create project based on template/"
  exit 1
fi

prog="$1"

# macOS default bash is too old, brew install bash to fix it.
prog=${prog,,}
# Prog=${prog^}
PROG=${prog^^}

Prog=""
for i in $(echo $prog | tr "_" "\n"); do
  i=${i^}
  Prog=$Prog$i
done

if [ -e "$prog" ]; then
  echo "ERROR: $prog already exists."
  exit 1
fi

if [[ "$prog" =~ .*"\.".* ]]; then
  echo "It's there."
fi

mkdir "$prog"

if [ -d "$prog" ]; then
  cp -R template/. $prog/

  # skip GNUmakefile
  for x in template.c CMakeLists.txt go.sh; do
    # sed -i not portable for macOS
    sed -e "s/TEMPLATE/$PROG/g" \
        -e "s/Template/$Prog/g" \
        -e "s/template/$prog/g" \
        $prog/$x > /tmp/$x
    mv /tmp/$x $prog/$x
  done

  for x in template.c; do
    new=$(echo $x | sed -e "s/template/$prog/g")
    mv $prog/$x $prog/$new
  done
fi
