/*
 * A simple TCP server to execute commands received from client.
 * It is for demo purpose only.
 * For simplicity, we can use telnet as the client.
 *
 * Copyright (c) 2010, zedware@gmail.com.
 */ 

#include "proto.h"

const char *progname = "server";

static void usage(void);

int main(int argc, char *argv[]) 
{
  	SOCKET 				listen_socket;
	unsigned short  	port;
  	struct sockaddr_in 	service;

	if (argc != 2)
	{
		usage();
		return 1;
	}

	port = atoi(argv[1]);
	if (port < 1024)
	{
		fprintf(stderr, "invalid port: %d\n", port);
		return 1;
	}

	if (winsock_startup() < 0)
		return 1;

  	listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  	if (listen_socket == -1) {
    	fprintf(stderr, "error at socket(): %d\n", errno);
    	return 1;
  	}

	memset(&service, 0, sizeof(service));
  	service.sin_family = AF_INET;
  	service.sin_addr.s_addr = inet_addr("127.0.0.1");
  	service.sin_port = htons(port);

  	if (bind(listen_socket, 
    	(struct sockaddr *) &service, 
  	  	sizeof(service)) == -1)
	{
    	fprintf(stderr, "bind() failed.\n");
    	close(listen_socket);
        return 1;
    }

    if (listen(listen_socket, 1) == -1)
      fprintf(stderr, "error listening on socket.\n", errno);

    fprintf(stdout, "listening on socket...\n");

	for ( ; ; )
	{
		struct sockaddr addr;
		SOCKET	accept_socket;
		int		addr_len;
		char	buf[BUFSZ];
		char	*p = buf;
		
		addr_len = sizeof(addr);
		accept_socket = accept(listen_socket, &addr, &addr_len);
		if (accept_socket == INVALID_SOCKET)
		{
			fprintf(stderr, "accept() failed: %d\n", WSAGetLastError());
		}
		else
		{
			fprintf(stdout, "connection accepted.\n");
			if (readline(accept_socket, buf, BUFSZ) < 0)
				fprintf(stderr, "readn() failed.");
			else
			{	
				fprintf(stdout, "command received: %s\n", buf);
				/* Server will be blocked by the child. */
				if (system(buf) < 0)
				{
					fprintf(stdout, "failed.\n");
				}
				else
				{
					fprintf(stdout, "ok.\n");
				}
			}
			
			close(accept_socket);
		}
	}
	
	winsock_cleanup();
	/* not free resources, let it be. */
	return 0;
}

static void usage(void)
{
	fprintf(stdout, "%s <port>\n", progname);
}

