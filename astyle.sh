#!/usr/bin/env bash

git ls-files | egrep '*\.(c|h)$' | xargs astyle --style=stroustrup --break-blocks \
    --pad-oper --pad-first-paren-out --unpad-paren --align-pointer=name

#    --keep-one-line-blocks --add-one-line-brackets
