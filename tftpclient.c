#include "tftp.h"


struct sockaddr_in getServerStruct()
{
  struct sockaddr_in serv_addr;

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;

  serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
  serv_addr.sin_port = htons(SERV_UDP_PORT);

  return serv_addr;
}


void getFile(char *filename)
{
  int sockfd;
  struct sockaddr_in serv_addr;
  FILE * file;

  //TODO handle all file errors
  file = fopen(filename, "wb");

  if(file == NULL)
  {
    printf("file is null\n");
    return;
  }

  sockfd = createUDPSocketAndBind(0);
  serv_addr = getServerStruct();

  if(sockfd < 0)
  {
    printf("Couldn't open socket\n");
    return;
  }

  if(!send_RRQ(sockfd, (struct sockaddr *) &serv_addr, filename, TFTP_SUPORTED_MODE))
  {
    printf("Error: couldn't send RRQ\n");
    return;
  }
  if(!recvFile(sockfd, (struct sockaddr *) &serv_addr, file))
  {
    printf("Error: didn't receive file\n");
    return;
  }
  fclose(file);
  return;
}


void putFile(char *filename)
{
  int sockfd;
  struct sockaddr_in serv_addr;
  PACKET packet;
  int result;
  FILE * file;
  file = fopen(filename, "rb");

  if(file == NULL)
  {
    printf("file is null\n");
    return;
  }

  sockfd = createUDPSocketAndBind(0);
  serv_addr = getServerStruct();

  if(sockfd < 0)
  {
    printf("Couldn't open socket\n");
    return;
  }

  if(!send_WRQ(sockfd, (struct sockaddr *) &serv_addr, filename, TFTP_SUPORTED_MODE))
  {
    printf("Error: couldn't send WRQ\n");
    return;
  }
  result = waitForPacket(sockfd, (struct sockaddr *) &serv_addr, TFTP_OPTCODE_ACK, &packet);
  if(result > 0) sendFile(sockfd, (struct sockaddr *) &serv_addr, file);
  else
  {
    printf("Couldn't sendfile()\n");
  }
  fclose(file);
  return;
}


int main(int argc, char *argv[])
{
  //TODO strotk_r
  //TODO port Numbers
  //TODO print usage
  if(argv[1][0] == '-')
  {

    switch(argv[1][1])
    {
      case 'w':
      case 'p': //put file
        putFile(argv[2]);
        break;
      case 'r':
      case 'g': //get file
        getFile(argv[2]);
        break;
      default: 
        printf("Not a valid action \n");
        break;
    }
  }
}
