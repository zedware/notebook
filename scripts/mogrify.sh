#!/usr/bin/env bash
# convert -size 768x1024 *.png x.pdf

start=897
end=922

left=460
top=170
right=0
bottom=150

for x in $(seq $start $end); do
   y="Screenshot ($x).png"
   echo $y
   mogrify -crop +$left+$top -crop -$right-$bottom +repage "$y"
done
