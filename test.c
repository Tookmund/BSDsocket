/* Simple test program */
#include <stdio.h>
#include <stdlib.h>
#include "BSDsocket.h"
#include <string.h>

#define PORT 8888
#define MESSAGE "Hello World!!"
#define MESSLEN 14
int main () {
	int listener = sock_listener(PORT,1);
	printf("listener created\n");
	int client = sock_connectto("127.0.0.1",PORT);
	printf("client created\n");
	int server = sock_accept(listener);
	char *message = MESSAGE;
	printf("\nSending: %s\n",message);
	sock_write(client,message,MESSLEN);
	char recivebuf[MESSLEN+1]; /* just to be safe... */
	memset(recivebuf,0,MESSLEN+1);
	sock_read(server,recivebuf,MESSLEN);
	printf("\nRecived: %s\n",recivebuf);
	return 0;
}
