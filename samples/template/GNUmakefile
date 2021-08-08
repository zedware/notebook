#
# prog=foo make -f GNUmakefile 
#
CC := gcc
CFLAGS := -std=c11 -g -O0 -Wall -Wextra -fsanitize=address -static-libasan -fno-omit-frame-pointer
LIBS := -lm

OBJS=$(prog).o 

all: $(prog)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(prog): $(OBJS)
	$(CC) $(CFLAGS) -o $(prog) $(OBJS) $(LIBS)

test: all
	./$(prog)

clean:
	rm -rf $(prog).o $(prog)

help:
	@echo "Usage: prog=hello make [clean|help|all|test]"

