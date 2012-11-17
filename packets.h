#ifndef PACKETS_H
#define PACKETS_H

#include <stdlib.h>
#include <string.h>
#include "tftp.h"

#define MAX_STRING_SIZE 1024
#define MAX_MODE_SIZE 8
#define MAX_DATA_SIZE 512

typedef struct {
    char filename[MAX_STRING_SIZE+1];
    char mode[MAX_STRING_SIZE+1];
} RRQ, WRQ;

typedef struct {
  u_int16_t blockNumber;
  char data[MAX_DATA_SIZE];
} DATA;

typedef struct {
  u_int16_t blockNumber;
} ACK;

typedef struct {
  u_int16_t errorCode;
  char message[MAX_STRING_SIZE];
} ERROR;

typedef struct {
  u_int16_t optcode;
  union {
    RRQ read_request;
    WRQ write_request;
    DATA data;
    ACK ack;
    ERROR error;
  };
} PACKET;


size_t charncpy(char *dest, const char *src, size_t n);

PACKET* getPacket(char * buffer);
size_t setPacket(const PACKET* packet, char * buffer);

u_int16_t getHostOrderShortFromNetwork(void * buff);
u_int16_t getNetworkOrderShortFromHost(u_int16_t hostshort, void * buff);

void printPacket(PACKET* packet);

#endif
