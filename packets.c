
#include "packets.h"

/* 
 * this function will rturn the appropriate  packet struct given a buffer 
 * pointer to the next packet recieved
 */
PACKET * getPacket(char * buffer)
{
  unsigned int optcode = (unsigned int)*buffer;

  PACKET * packet = malloc(sizeof(PACKET));
  packet->optcode = ntohs(optcode);
  char* dataOffset = &buffer[2];
  size_t n;

  switch (optcode)
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
      packet->data.blockNumber = ntohs((unsigned int)*dataOffset);
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

    return len;
}

