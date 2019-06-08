/*
 * Shared subroutines for client/server.
 *
 * Copyright (c) 2010, zedware@gmail.com.
 */ 

#include "proto.h"

#ifdef WIN32
int winsock_startup(void)
{
	WORD	wVersionRequested;
	WSADATA wsaData;

    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		fprintf(stderr, "cannot load winsock library: %d\n", GetLastError());
		return -1;
	}

	return 0;
}

void winsock_cleanup(void)
{
	WSACleanup();
}
#endif

int readn(SOCKET fd, char *buf, int len)
{
	int pos;

	assert(fd != INVALID_SOCKET);
	assert(NULL != buf);
	assert(len > 0);

	pos = 0;
	while (pos < len)
	{
		int rc;

		rc = recv(fd, buf + pos, len, 0);
		if (rc > 0)
		{
			pos += rc;
		}
		else if (rc == 0)
		{
			return -1;
		}
		else /* if (SOCKET_ERROR == rc) */
		{
			if (errno == EINTR || errno == EWOULDBLOCK)
				continue;

			return -1;
		}
	}

	return pos;
}

int writen(SOCKET fd, const char *buf, int len)
{
	int	pos;

	assert(fd != INVALID_SOCKET);
	assert(NULL != buf);
	assert(len > 0);
	
	pos = 0;
	while (pos < len)
	{
		int rc;

		rc = send(fd, buf + pos, len - pos, 0);
		if (rc > 0)
		{
			pos += rc;
		}
		else /* if (rc == SOCKET_ERROR) */
		{	
			if (errno == EINTR)
				continue;

			return -1;
		}
	}
	
	return pos;
}


int readline(SOCKET fd, char *str, int len)
{
	char	ch;
	int		rc;
	char	*p = str;
	
	assert(INVALID_SOCKET != fd);
	assert(str != NULL);
	assert(len > 0);
	
	for ( ; ; )
	{
		rc = recv(fd, &ch, 1, 0);
		if (rc > 0)
		{
			if (ch == '\r' || ch == '\n')
			{
				break;
			}

			if (p - str < len - 1)
			{
				*p++ = ch;
			}
			else
			{
				fprintf(stderr, "extra character received: %c\n", ch);
			}				
		}
		else if (rc == 0)
			return -1;
		else
		{
			if (errno == EINTR)
				continue;
				
			return -1;
		}
	}

	*p = '\0';
	return 0;	
}

int writeline(SOCKET fd, const char *str)
{
	int	len;
	
	assert(fd != INVALID_SOCKET);
	
	if (str == NULL)
		return 0;
	
	len = (int) strlen(str);
	if (writen(fd, str, len) < 0)
		return -1;
/*
	if (writen(fd, "\n", 1) < 0)
		return -1;
*/	
	return 0;
}
