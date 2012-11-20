#ifndef TFTP_H
#define TFTP_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "packets.h"
#include "test.h"
#include "common.h"
#include "pong.h"

//port 6100 + group 6
#define SERV_UDP_PORT   6106

#define SERV_HOST_ADDR  "127.0.0.1"

#define MAX_TFTP_CLIENTS 100

char *progname;

//max packet size
#define BUFSIZE 2048

#endif
