#!/usr/bin/env bash

# %04 pad the name to make it sort in order.
#  img2pdf.sh to convert them into a single pdf.
#  liberoffice to create a new presentation, choose Insert->Media->Photo Album
# Both rely on the sort order of files.
ffmpeg -i x.mp4 -vf fps=1 out%04d.png

#Output one image every second:
#
#ffmpeg -i input.mp4 -vf fps=1 out%d.png
#Output one image every minute:
#
#ffmpeg -i test.mp4 -vf fps=1/60 thumb%04d.png
#Output one image every 10 minutes:
#
#ffmpeg -i test.mp4 -vf fps=1/600 thumb%04d.png
#
