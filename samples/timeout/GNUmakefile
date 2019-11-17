# This assumes the MySQL software is installed in /usr/local/mysql
#inc      := /usr/local/mysql/include/mysql
#lib      := /usr/local/mysql/lib

# If you have not installed the MySQL software yet, try this instead
topdir   := /home/wenliang/Workspace/sandbox/mysql-5.1.52
inc      := $(topdir)/include
# /libmysqld       for embedded server
# /libmysql/.libs  for mysqlclient using shared libraryr
lib      := $(topdir)/libmysql/.libs

CXX      := g++
CPPFLAGS := -I$(inc) -D_THREAD_SAFE -D_REENTRANT
CXXFLAGS := -g -Wall
LDFLAGS  := -Wl,-rpath,$(lib)
# You can change -lmysqld to -lmysqlclient to use the
# client/server library
LDLIBS    = -L$(lib) -lmysqlclient -lm -lcrypt -lz -lrt -ldl

ifneq (,$(shell grep FreeBSD /COPYRIGHT 2>/dev/null))
# FreeBSD
LDFLAGS += -pthread
else
# Assume Linux
LDLIBS += -pthread
endif

# This works for simple one-file test programs
sources := $(wildcard *.cc)
objects := $(patsubst %cc,%o,$(sources))
targets := $(basename $(sources))

all: $(targets)

check: all
	#export LD_LIBRARY_PATH=/home/wenliang/Workspace/sandbox/mysql-5.1.52/libmysql/.libs/ && ./main
	./main

clean:
	rm -f $(targets) $(objects) *.core

