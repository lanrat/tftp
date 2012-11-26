#include "tftp.h"


struct sockaddr_in getServerStruct(int port)
{
  struct sockaddr_in serv_addr;

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;

  serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
  serv_addr.sin_port = htons(port);

  return serv_addr;
}


void getFile(int port, char *filename)
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
  serv_addr = getServerStruct(port);

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


void putFile(int port, char *filename)
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
  serv_addr = getServerStruct(port);

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
  int port = SERV_UDP_PORT;
  int argOffset = 1;
  char* filename;
  
  if (argc == 5 && argv[1][0] == '-' && argv[1][1] == 'p')
  {
    //change the port
    port = atoi(argv[2]);
    argOffset +=2;
  }

  if(( argc == 3 || argc == 5) && argv[argOffset][0] == '-')
  {
    switch(argv[argOffset][1])
    {
      case 'w':
        putFile(port,argv[argOffset+1]);
        return;
      case 'r':
        getFile(port,argv[argOffset+1]);
        return;
      default: 
        printf("Unknown action [%s] \n",argv[argOffset]);
        break;
    }
    //print usage
  }
  printf("Usage: %s [-p port] (-w putfile || -r getFile)\n",argv[0]);
}
