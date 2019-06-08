#ifndef PROTO_H
#define PROTO_H

#ifdef WIN32
#include <windows.h>
#pragma warning (disable: 4996)
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

/* Buffer size of command. */
#define BUFSZ				8192

#ifdef WIN32

#define close				closesocket
#define EWOULDBLOCK			WSAEWOULDBLOCK

#else

typedef int		SOCKET;

#define INVALID_SOCKET		-1 
#define WSAGetLastError()	errno
#endif

#ifdef WIN32
extern int winsock_startup(void);
extern void winsock_cleanup(void);
#endif

extern int readn(SOCKET fd, char *buf, int len);
extern int writen(SOCKET fd, const char *buf, int len);
extern int readline(SOCKET fd, char *str, int len);
extern int writeline(SOCKET fd, const char *str);

#endif /* PROTO_H */
