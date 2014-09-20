#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <limits.h>
#include <string.h>
#include "BSDsocket.h"

int sock_listener(unsigned short portnum,int max_connect) {
	char *myname = "127.0.0.1";
	int s; /* socket */
	struct sockaddr_in sa;
	memset(&sa,0,sizeof(sa)); /* wipe sock address struct */
	//getnameinfo((const struct sockaddr *) &sa,sizeof(sa),myname,sizeof(myname),NULL,0,0);
	s = socket(AF_INET,SOCK_STREAM,0);
	if (s < 0) {
		perror("create socket listener");
		return(-1);
	}
	sa.sin_family = AF_INET;
	sa.sin_port = htons(portnum);
	inet_pton(AF_INET,myname,&sa.sin_addr);
	int retval = bind(s,(struct sockaddr *) &sa,sizeof(struct sockaddr));
	if (retval < 0) {
		printf("bind socket listener to %u", portnum);
		perror("");
		close(s);
		return(-1);
	}
	listen(s,max_connect);
	return(s);
}

int sock_accept(int s) {
	int t;
	t = accept(s,NULL,NULL);
	if (t < 0) {
		perror("Connection");
	}
	return(t);
}

int sock_connectto(char *hostname,unsigned short portnum) {
	struct sockaddr_in sa;
	int s;
	/* getnameinfo((const struct sockaddr *) &sa,sizeof(sa),hostname,sizeof(hostname),NULL,0,0); */
	sa.sin_family = AF_INET;
	s = socket(sa.sin_family,SOCK_STREAM,0);
	if(s < 0) {
		perror("");
		printf("create connector to %s:%uh",hostname,portnum);
		return(-1);
	}
	sa.sin_port = htons(portnum);
	inet_pton(AF_INET,hostname,&sa.sin_addr);
	int retval = connect(s,(struct sockaddr *) &sa,sizeof(sa));
	if(retval < 0) {
		perror("");
		printf("connect connector to %s:%uh",hostname,portnum);
		close(s);
		return(-1);
	}
	return(s);
}
int sock_read(int s, char *buf, int n) {
	int bcount = 0;
	int br = 0;
	while (bcount < n) {
		br = read(s, buf,n-bcount);
		if (br > 0) {
			bcount += br;
			buf += br; /* increment buffer pointer so as not to overwrite previous data */
		}
		else if (br < 0)
			return(-1);
	}
	return(bcount);
}
int sock_write(int s, char *buf, int n) {
	int bcount = 0;
	int br = 0;
	while (bcount < n) {
		br = write(s, buf,n-bcount);
		if (br > 0) {
			bcount += br;
			buf += br; /* increment buffer pointer so as not to overwrite previous data */
		}
		else if (br < 0)
			return(-1);
	}
	return(bcount);
}
