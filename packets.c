
#include "packets.h"

/* 
 * this function will rturn the appropriate  packet struct given a buffer 
 * pointer to the next packet recieved
 */
PACKET * getPacket(char * buffer)
{

  PACKET * packet = malloc(sizeof(PACKET));
  packet->optcode = getHostOrderShortFromNetwork(buffer);
  char* dataOffset = &buffer[2];
  size_t n;

  switch (packet->optcode)
  {
    case 1: //read
      n = charncpy(packet->read_request.filename,dataOffset,MAX_STRING_SIZE);
      dataOffset += n;
      charncpy(packet->read_request.mode,dataOffset,MAX_STRING_SIZE);
      break;
    case 2: //write
      n = charncpy(packet->write_request.filename,dataOffset,MAX_STRING_SIZE);
      dataOffset += n;
      charncpy(packet->write_request.mode,dataOffset,MAX_STRING_SIZE);
      break;
    case 3: //data
      packet->data.blockNumber = ntohs((u_int16_t)*dataOffset);
      dataOffset +=2;
      charncpy(packet->data.data,dataOffset, MAX_STRING_SIZE);
      break;
    case 4: //ack
      packet->ack.blockNumber = ntohs(*dataOffset);
      break;
    case 5: //error
      packet->error.errorCode = ntohs(*dataOffset);
      dataOffset +=2;
      charncpy(packet->error.message,dataOffset, MAX_STRING_SIZE);
      break;
    default:
      free(packet);
      return NULL;
  }
  //done
  return packet;
}

/* prbly very similar to strncpy but returns the size of the string copied */
size_t charncpy(char *dest, const char *src, size_t n)
{
    size_t i,len;

    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    len = i;
    for ( ; i < n; i++)
    {
        dest[i] = '\0';
    }

    return len+1;
}

/* A little function to get the host ordering of a short from a pointer */
u_int16_t getHostOrderShortFromNetwork(void * buff)
{
    u_int16_t data;
    memcpy(&data,buff,sizeof(u_int16_t));
    return ntohs(data);
}

/* prints the packet and all fields for debugging */
void printPacket(PACKET* packet)
{
  if (packet == NULL)
  {
      printf("Null packet");
      return;
  }

  printf("Optcode: [%u] ",packet->optcode);

  switch (packet->optcode)
  {
    case 1: //read
      printf("READ\n");
      printf("Filename: %s\n",packet->read_request.filename);
      printf("Mode: %s\n",packet->read_request.mode);
      break;
    case 2: //write
      printf("WRITE\n");
      printf("Filename: %s\n",packet->write_request.filename);
      printf("Mode: %s\n",packet->write_request.mode);
      break;
    case 3: //data
      printf("DATA\n");
      printf("Block #: %u\n",packet->data.blockNumber);
      break;
    case 4: //ack
      printf("ACK\n");
      printf("Block #: %u\n",packet->ack.blockNumber);
      break;
    case 5: //error
      printf("ERROR\n");
      printf("ErrorCode: %u\n",packet->error.errorCode);
      printf("ErrorMessage: %s\n",packet->error.message);
      break;
    default:
      printf("Unknown Packet Type\n");
      break;
  }
}
