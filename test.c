/* Simple test program */
#include <stdio.h>
#include <stdlib.h>
#include "BSDsocket.h"

#define PORT 8888
#define MESSAGE "Hello World!!"

int main () {
	int listener = sock_listener(PORT,1);
	int client = sock_connectto("localhost",PORT);
	int server = sock_accept(listener);
	
	char *message = MESSAGE;
	printf("Sending: %s",message);
	sock_write(client,message,sizeof(message));
	char recivebuf[sizeof(message)+1]; /* just to be safe... */
	memset(recivebuf,0,sizeof(recivebuf));
	sock_read(server,recivebuf,sizeof(message));
	printf("Recived: %s",recivebuf);
	return 0;
}
