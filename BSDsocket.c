#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <limits.h>
#include <string.h>
#include "BSDsocket.h"

#define HOST_NAME_MAX 255 /* shouldn't be necessary and is probably dangerous */
struct hostent *sock_gethostbyname(char *name) {
	struct hostent *hp = gethostbyname(name/* ,HOST_NAME_MAX */);
	if (hp == NULL) {
		/* error checking as described by man page
		http://linux.die.net/man/3/gethostbyname */
		switch(h_errno) {
			case HOST_NOT_FOUND:
				printf("The specified host (%s) is unknown\n",name);
				break;
			/* case NO_ADDRESS: */
			case NO_DATA:
				printf("The requested name (%s) is valid but does not have an IP address\n",name);
				break;
			case NO_RECOVERY:
			printf("A nonrecoverable name server error occurred\n");
				break;
			case TRY_AGAIN:
				printf("A temporary error occurred on an authoritative name server. Try again later\n");
			default:
				break;
		}
	}
	return(hp);
}

int sock_listener(unsigned short portnum,int max_connect) {
	char myname[HOST_NAME_MAX+1]; 
	int s; /* socket */
	struct sockaddr_in sa;
	struct hostent *hp;
	memset(&sa,0,sizeof(sa)); /* wipe sock address struct */
	hp = sock_gethostbyname(myname);
	if (hp == NULL)
		return(-1);
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons(portnum);
	s = socket(AF_INET,SOCK_STREAM,0);
	if (s < 0) {
		perror("create socket listener");
		return(-1);
	}
	int retval = bind(s,(struct sockaddr *) &sa,sizeof(struct sockaddr));
	if (retval < 0) {
		printf("bind socket listener to %uh", portnum);
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
	struct hostent *hp;
	int s;
	hp = sock_gethostbyname(hostname);
	if(hp == NULL) {
		return(-1);
	}
	memset(&sa,0,sizeof(sa));
	memcpy((char *)&sa.sin_addr,hp->h_addr_list[0],hp->h_length);
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons((unsigned short)portnum);
	s = socket(hp->h_addrtype,SOCK_STREAM,0);
	if(s < 0) {
		perror("");
		printf("create connector to %s:%uh",hostname,portnum);
		return(-1);
	}
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
