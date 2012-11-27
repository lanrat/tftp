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

typedef int bool;
#define true 1
#define false 0

#include "packets.h"
#include "common.h"
#include "pong.h"
#include "filetransfer.h"

//port 6100 + group 6
#define SERV_UDP_PORT   6106

#define SERV_HOST_ADDR  "127.0.0.1"

#define MAX_TFTP_CLIENTS 100
#define MAX_TFTP_TIMEOUTS 10
#define TFTP_TIMEOUT_DURATION 5 //seconds

//we only support octet mode
#define TFTP_SUPORTED_MODE "octet"

//max packet size
#define BUFSIZE 2048

//for testing
#define DEBUG true

#endif
