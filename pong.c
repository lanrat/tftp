#include "pong.h"


bool send_RRQ(int sockfd, struct sockaddr_in* sockInfo, char* filename, char* mode)
{
  PACKET packet;
  char buffer[BUFSIZE];
  size_t n;

  packet.optcode = TFTP_OPTCODE_RRQ;
  charncpy(packet.read_request.filename,filename,MAX_STRING_SIZE);
  charncpy(packet.read_request.mode,mode,MAX_MODE_SIZE);

  n = serializePacket(&packet,buffer);
  
  return (sendto(sockfd,buffer,n,0,(struct sockaddr *)sockInfo,sizeof(struct sockaddr)) >= 0);
}

bool send_WRQ(int sockfd, struct sockaddr_in* sockInfo, char* filename, char* mode)
{
  PACKET packet;
  char buffer[BUFSIZE];
  size_t n;

  packet.optcode = TFTP_OPTCODE_WRQ;
  charncpy(packet.read_request.filename,filename,MAX_STRING_SIZE);
  charncpy(packet.read_request.mode,mode,MAX_MODE_SIZE);

  n = serializePacket(&packet,buffer);

  return (sendto(sockfd,buffer,n,0,(struct sockaddr *)sockInfo,sizeof(struct sockaddr)) >= 0);
}

bool send_data(int sockfd, struct sockaddr_in* sockInfo, u_int16_t blockNumber, char* data, size_t data_size)
{
  PACKET packet;
  char buffer[BUFSIZE];
  size_t n;

  packet.optcode = TFTP_OPTCODE_DATA;
  packet.data.blockNumber = blockNumber;
  packet.data.dataSize = data_size;
  charncpy(packet.data.data,data,data_size);
  
  n = serializePacket(&packet,buffer);
  
  return (sendto(sockfd,buffer,n,0,(struct sockaddr *)sockInfo,sizeof(struct sockaddr*)) >= 0);
}

//sends the arror message to the socket with the provided errorcode and message
bool send_error(int sockfd, struct sockaddr_in* sockInfo, u_int16_t errorCode, char* error_message)
{
  PACKET packet;
  char buffer[BUFSIZE];
  size_t n;

  packet.optcode = TFTP_OPTCODE_ERR;
  packet.error.errorCode = errorCode;
  strncpy(packet.error.message, error_message,MAX_STRING_SIZE);

  n = serializePacket(&packet,buffer);
  
  return (sendto(sockfd,buffer,n,0,(struct sockaddr *)sockInfo,sizeof(struct sockaddr)) >= 0);
}

//send an ack reply back to somewhere
bool send_ack(int sockfd, struct sockaddr_in* sockInfo, u_int16_t blockNumber)
{
  PACKET packet;
  char buffer[BUFSIZE];
  size_t n;

  packet.optcode = TFTP_OPTCODE_ACK;
  packet.ack.blockNumber = blockNumber;

  n = serializePacket(&packet,buffer);
  
  return (sendto(sockfd,buffer,n,0,(struct sockaddr *)sockInfo,sizeof(struct sockaddr)) >= 0);
}

