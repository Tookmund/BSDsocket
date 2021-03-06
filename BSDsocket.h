#ifndef _BSDSOCKET_H_
#define _BSDSOCKET_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>

int sock_listener(unsigned short portnum,int max_connect);
int sock_accept(int s);
int sock_connectto(char *hostname,unsigned short portnum);
int sock_read(int s, char *buf, int n);
int sock_write(int s, char *buf, int n);

#endif
