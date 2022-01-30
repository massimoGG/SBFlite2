#ifndef __ethernet_h_
#define __ethernet_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

const char *IP_Broadcast = "239.12.255.254";

extern int packetposition;
extern int MAX_CommBuf;

extern int debug;
extern int verbose;

extern int sock;
extern struct sockaddr_in addr_in, addr_out;

int ethConnect(short port);
int ethClose(void);
int getLocalIP(unsigned char IPAddress[4]);
int ethSend(unsigned char *buffer, const char *toIP);
int ethRead(unsigned char *buf, unsigned int bufsize);

#endif
