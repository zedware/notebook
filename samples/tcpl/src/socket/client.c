/*
 * A simple TCP client to send commands to TCP server.
 * It is for demo purpose only.
 * For simplicity, we can use telnet as the client.
 *
 * Copyright (c) 2010, zedware@gmail.com.
 */ 

#include "proto.h"

static void usage(void);

int main(int argc, char *argv[]) 
{
 	char 			*host;
	unsigned short  port;
	SOCKET			fd;
  	struct addrinfo *addr = NULL;
	struct sockaddr_in 	service;
	int				i;

	if (argc <= 3)
	{
		usage();
		return 1;
	}

	host = strdup(argv[1]);
	if (host == NULL)
		return 1;
		
	port = atoi(argv[2]);
	if (port < 1024)
	{
		printf("invalid port: %d\n", port);
		return 1;
	}

	if (winsock_startup() < 0)
		return -1;

  	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  	if (fd == -1) 
	{
    	fprintf(stderr, "socket() failed: %d\n", errno);
    	return 1;
  	}

  	service.sin_family = AF_INET;
  	service.sin_addr.s_addr = inet_addr(host);
  	service.sin_port = htons(port);

	if (connect(fd, (struct sockaddr*) &service, sizeof(service)) < 0)
	{
    	fprintf(stderr, "connect() failed: %d\n", errno);
		return 1;
	}

	for (i = 3; i < argc; i++)
	{
		writen(fd, argv[i], (int) strlen(argv[i]));
		writen(fd, " ", 1);
	}
	writeline(fd, "\n");
	
	close(fd);
	free(host);	
	winsock_cleanup();
	return 0;
}

static void usage(void)
{
	printf("cmd <host> <port> <cmd>\n");
}

