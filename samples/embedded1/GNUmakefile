# This assumes the MySQL software is installed in /usr/local/mysql
#inc      := /usr/local/mysql/include/mysql
#lib      := /usr/local/mysql/lib

# If you have not installed the MySQL software yet, try this instead
topdir   := /u01/my4006
inc      := $(topdir)/include
lib      := $(topdir)/libmysqld

CXX      := g++
CPPFLAGS := -I$(inc) -D_THREAD_SAFE -D_REENTRANT
CXXFLAGS := -g -Wall
LDFLAGS  := 
# You can change -lmysqld to -lmysqlclient to use the
# client/server library
LDLIBS    = -L$(lib) -lmysqld -lm -lcrypt -ldl -lz -lrt

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

clean:
	rm -f $(targets) $(objects) *.core

