
CC=gcc
CFLAGS=-Wall

OBJS=$(prog).o getline.o

all: $(prog)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(prog): $(OBJS)
	$(CC) $(CFLAGS) -o $(prog) $(OBJS)

test: all
	sh -x ./test.sh $(prog)

clean:
	@rm -rf $(prog).o $(prog)
