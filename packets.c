
#include "packets.h"

/* 
 * this function will rturn the appropriate  packet struct given a buffer 
 * pointer to the next packet recieved
 */
PACKET * getPacket(char * buffer)
{

  PACKET * packet = malloc(sizeof(PACKET));
  bzero(packet,sizeof(PACKET));
  packet->optcode = getHostOrderShortFromNetwork(buffer);
  char* dataOffset = &buffer[2];
  size_t n;

  switch (packet->optcode)
  {
    case 1: //read
      n = charncpy(packet->read_request.filename,dataOffset,MAX_STRING_SIZE);
      dataOffset += n;
      charncpy(packet->read_request.mode,dataOffset,MAX_MODE_SIZE);
      break;
    case 2: //write
      n = charncpy(packet->write_request.filename,dataOffset,MAX_STRING_SIZE);
      dataOffset += n;
      charncpy(packet->write_request.mode,dataOffset,MAX_MODE_SIZE);
      break;
    case 3: //data
      packet->data.blockNumber = getHostOrderShortFromNetwork(dataOffset);
      dataOffset +=2;
      charncpy(packet->data.data,dataOffset, MAX_DATA_SIZE);
      break;
    case 4: //ack
      packet->ack.blockNumber = getHostOrderShortFromNetwork(dataOffset);
      break;
    case 5: //error
      packet->error.errorCode = getHostOrderShortFromNetwork(dataOffset);
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

/* reads data from a packet struct into a buffer to send over the wire
 * returns the size of the buffer to send */
size_t setPacket(const PACKET* packet, char * buffer)
{
  if (buffer == NULL || packet == NULL)
  {
    return 0;
  }

  size_t n;
  char* dataOffset;
  
  *((u_int16_t*)buffer) = getNetworkOrderShortFromHost(packet->optcode,NULL);
  dataOffset = buffer + 2; //skip over the optcode

  switch (packet->optcode)
  {
    case 1: //read
      n = charncpy(dataOffset,packet->read_request.filename,MAX_STRING_SIZE);
      dataOffset += n;
      charncpy(dataOffset,packet->read_request.mode,MAX_MODE_SIZE);
      n = sizeof(RRQ);
      break;
   case 2: //write
      n = charncpy(dataOffset,packet->read_request.filename,MAX_STRING_SIZE);
      dataOffset += n;
      charncpy(dataOffset,packet->read_request.mode,MAX_MODE_SIZE);
      n = sizeof(WRQ);
      break;
    case 3: //data
      *((u_int16_t*)dataOffset) = getNetworkOrderShortFromHost(packet->data.blockNumber,NULL);
      dataOffset +=2; //skip over block number
      charncpy(dataOffset,packet->data.data,MAX_DATA_SIZE);
      n = sizeof(DATA);
      break;
    case 4: //ack
      *((u_int16_t*)dataOffset) = getNetworkOrderShortFromHost(packet->ack.blockNumber,NULL);
      n = sizeof(ACK);
      break;
    case 5: //error
      *((u_int16_t*)dataOffset) = getNetworkOrderShortFromHost(packet->ack.blockNumber,NULL);
      dataOffset +=2; //skip over errorCode
      charncpy(dataOffset,packet->error.message,MAX_STRING_SIZE);
      n = sizeof(ERROR);
      break;
    default:
      return 0;
  }
  //done
  return n + sizeof(u_int16_t); //sizeof(PACKET)
 
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

/* A function to get the network ordering of a short from a pointer */
u_int16_t getNetworkOrderShortFromHost(u_int16_t hostshort, void * buff)
{
    u_int16_t data = htons(hostshort);
    if (buff != NULL)
    {
        memcpy(buff,&data,sizeof(u_int16_t));
    }
    return data;
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
