#!/usr/bin/env bash

for x in *.png; do
   convert "$x" -background white -alpha remove -alpha off "x-$x"
done

pwd=`pwd`
output=`basename $pwd`
echo "Output: $output.pdf"

img2pdf --output "$output.pdf" x-*.png
rm -rf x-*.png
