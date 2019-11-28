@echo off

set files=UNIX Version Windows
for %%x in (%files%) do dot -Nfontname=simsun -T png -o %%x.png %%x.dot

rem rem English only
rem set files=test1 test2
rem for %%x in (%files%) do dot -Gcharset=latin1 -T png -o %%x.png %%x.dot
rem
rem rem Chinese and font name
rem rem 1. input Chinese characters using utf-8.
rem rem 2. use font in CLI and .dot files.
rem set files=Education-utf8
rem for %%x in (%files%) do dot -Nfontname=simsun -T png -o %%x.png %%x.dot

pause
