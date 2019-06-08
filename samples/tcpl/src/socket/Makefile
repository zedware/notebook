
all: server client

server: server.o proto.o
	$(CC) $(CFLAGS) -o $@ $? $(LIBS)

client: client.o proto.o
	$(CC) $(CFLAGS) -o $@ $? $(LIBS)

clean:
	@rm -rf server.o server client.o client proto.o
