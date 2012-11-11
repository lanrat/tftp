#include <stdlib.h>

#define MAX_STRING_SIZE 2048

typedef struct {
    char filename[MAX_STRING_SIZE];
    char mode[MAX_STRING_SIZE];
} RRQ, WRQ;

typedef struct {
  unsigned int blockNumber;
  char data[512];
} DATA;

typedef struct {
  unsigned int blockNumber;
} ACK;

typedef struct {
  unsigned int errorCode;
  char message[MAX_STRING_SIZE];
} ERROR;

typedef struct {
  unsigned int optcode;
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
