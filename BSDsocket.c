#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>

/* Establish a Socket.
Type:
4 = AF_INET
0 = AF_UNIX
*/
int establish(unsigned short port, int type) {
	if (type == 4) {
		char myname[MAXHOSTNAME+1];
		int    s;
		struct sockaddr_in sa;
		struct hostent *hp;
		memset(&sa, 0, sizeof(struct sockaddr_in)); 
		gethostname(myname, MAXHOSTNAME);           
		hp= gethostbyname(myname);                  
		if (hp == NULL) return(-1);
		sa.sin_family= hp->h_addrtype;              
		sa.sin_port= htons(portnum);                
		if ((s= socket(AF_INET, SOCK_STREAM, 0)) < 0) return(-1);
		if (bind(s,&sa,sizeof(struct sockaddr_in)) < 0) {
			close(s);
				return(-1);                         
  	}
  	listen(s, 3);                               
  	return(s);
  	}
  	else if (type == 0) {
  		/* Not sure how to do this yet */
  	}
}
int get_connection(int s) { 
	int t;
  	if ((t = accept(s,NULL,NULL)) < 0) return(-1);
  	return(t);
}
int call_socket(char *hostname, unsigned short portnum) { 
	struct sockaddr_in sa;
	struct hostent     *hp;
	int a, s;

	if ((hp= gethostbyname(hostname)) == NULL) { /* do we know the host's */
    	errno= ECONNREFUSED;                       /* address? */
    	return(-1);                                /* no */
  	}
  	
  	memset(&sa,0,sizeof(sa));
  	memcpy((char *)&sa.sin_addr,hp->h_addr,hp->h_length); /* set address */
  	sa.sin_family= hp->h_addrtype;
  	sa.sin_port= htons((u_short)portnum);

	if ((s= socket(hp->h_addrtype,SOCK_STREAM,0)) < 0)   /* get socket */
		return(-1);
	if (connect(s,&sa,sizeof sa) < 0) {                  /* connect */
		close(s);
		return(-1);
	}
	return(s);
}
int read_sock ()
