//#include "pong.h"
#include "tftp.h"

volatile bool timeout;

//returns the handle to a socket on the given port
//if the port is 0 then the OS will pick an avaible port
int createUDPSocketAndBind(int port)
{
  int sockfd;
  struct sockaddr_in serv_addr;
  
  //create a socket
  sockfd = socket(PF_INET, SOCK_DGRAM, 0);

  //return -1 on error
  if (sockfd == -1)
  {
    return -1;
  }
  
  //zero out the struct
  bzero((char*) &serv_addr, sizeof(serv_addr));

  //create the struct to bind on
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  //bind to it
  if (bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
  {
    //error
    return -1;
  }

  return sockfd;
}

bool send_RRQ(int sockfd, struct sockaddr* sockInfo, char* filename, char* mode)
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

bool send_WRQ(int sockfd, struct sockaddr* sockInfo, char* filename, char* mode)
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

bool send_data(int sockfd, struct sockaddr* sockInfo, u_int16_t blockNumber, char* data, size_t data_size)
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
bool send_error(int sockfd, struct sockaddr* sockInfo, u_int16_t errorCode, char* error_message)
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
bool send_ack(int sockfd, struct sockaddr* sockInfo, u_int16_t blockNumber)
{
  PACKET packet;
  char buffer[BUFSIZE];
  size_t n;

  packet.optcode = TFTP_OPTCODE_ACK;
  packet.ack.blockNumber = blockNumber;

  n = serializePacket(&packet,buffer);
  
  return (sendto(sockfd,buffer,n,0,(struct sockaddr *)sockInfo,sizeof(struct sockaddr)) >= 0);
}


// THIS CODE IS FOR TIMEOUTS!
void handler(int sig)
{
  timeout = true;
  alarm(0);
}


//3 possible outcomes
//timeout: retrn -1
//got desired packet: size of pakcket returned
//error recieved: 0
int waitForPacket(int sockfd, struct sockaddr* cli_addr, u_int16_t optcode, PACKET* packet)
{
  char buffer[BUFSIZE];
  size_t n;
  size_t cli_size = sizeof(cli_addr);
  timeout = false;

  signal(SIGALRM, handler);
  alarm(TFTP_TIMEOUT_DURATION);

  do{
    n = recvfrom(sockfd, buffer, BUFSIZE, 0, cli_addr, (socklen_t *)&cli_size);
    unserializePacket(buffer, n, packet);
    if (packet->optcode == optcode)
    {
      alarm(0);
      return n;
    }
    else if(packet->optcode == TFTP_OPTCODE_ERR)
    {
      alarm(0);
      return 0;
    }
  } while(!timeout);

  //will only exit while loop if there had been a timeout that stopped the loop
  alarm(0);
  return -1;
}
