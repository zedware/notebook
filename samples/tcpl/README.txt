A small project should contains at least these parts:
1. README files.
2. project files, may include Makefile, autoconfig files.
3. source files.
4. test scripts and test cases.

Before you say it is done, test your program intensively.

Usage:

$ export prog=1-21; make; make test;

It is a good start to learn some basic techniques from this
sample:

1. Write basic makefiles.
2. Write program to read from stdin and write to stdout.
   It can then support pipes: cat foo.c | a.out | ...
3. Write shell scripts to test the cmd line programs.
