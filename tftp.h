
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>


typedef int bool;
#define true 1
#define false 0

//port 6100 + group 6
#define SERV_UDP_PORT   6106

#define SERV_HOST_ADDR  "127.0.0.1"

char *progname;

//TODO thic should be a smaller value
#define BUFSISE 1024

