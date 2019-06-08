
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <float.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>
       
static void bye(void)
{
 	printf("    atexit()\n");
}

struct hostent * gethostname(char * host)
{
    struct  hostent hostbuf, *hp;
    size_t  buf_len;
    char   *tmp_buf;
    int     res;
    int     herr;

    buf_len = 1024;
    tmp_buf = (char *) malloc(buf_len);
	if (tmp_buf == NULL)
		return NULL;

    while ((res = gethostbyname_r(host, &hostbuf,
								  tmp_buf, buf_len, 
                                  &hp, &herr)) == ERANGE)
    {
        buf_len *= 2;
        tmp_buf = realloc(tmp_buf, buf_len);
		if (tmp_buf == NULL)
			return NULL;
    }
   
	free(tmp_buf); 
	if (res || hp == NULL)
      return NULL;
  
	return hp;
}


int main(void)
{
    struct sockaddr_in addr;
    struct hostent *host;
    
	printf("inet_ntoa() is in LSB libc, but inet_aton() is not.\n");  
    if (inet_aton("127.0.0.1", &addr.sin_addr) == 0)  
        perror("    failed");  
	else 
	    printf("    ok.\n");
    
	printf("__isinf() is in LSB libc, but isinf() is not.\n");  
    printf("    isinf((double) 4.567) = %f\n",
			 	isinf((double) 4.567));

    printf("gethostbyname() is in LSB libc, but gethostbyname_r() is not.\n");
    host = gethostname("localhost");
    if (host != NULL)
	    printf("    hostname of \"localhost\" is %s\n",
				host->h_name);
	else
		printf("    failed.\n");
    
    printf("atexit() is not in LSB libc.\n");
    atexit(bye);

    exit(EXIT_SUCCESS);
}

