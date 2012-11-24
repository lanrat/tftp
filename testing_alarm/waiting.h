#ifndef WAITING_H
#define WAITING_H

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>

typedef int bool;
#define true 1
#define false 0

bool waiting(int time);
void handler(int sig);


#endif