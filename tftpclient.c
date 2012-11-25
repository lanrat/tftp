#include "tftp.h"



void readFile(int sockfd, struct sockaddr_in serv_addr,char *filename)
{ 
  FILE * file;
  file = fopen(filename, "rb");

  if(file == NULL)
  {
    printf("file is null\n");
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

void writeFile(int sockfd, struct sockaddr_in serv_addr, char *filename)
{
  PACKET packet;
  int result;
  FILE * file;
  file = fopen(filename, "wb");

  if(file == NULL)
  {
    printf("file is null\n");
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


//
int main(int argc, char *argv[])
{
  int sockfd;

  struct sockaddr_in      serv_addr;

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family      = AF_INET;

  serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
  serv_addr.sin_port        = htons(SERV_UDP_PORT);
  
  sockfd = createUDPSocketAndBind(0);
  if(sockfd == -1)
  {
    printf("Couldn't create sockfd\n");
    return;
  }
  

  printf("Created sockfd and created file handle\n");
  if(argv[1][0] == '-')
  {

    switch(argv[1][1])
    {
      case 'w':
        writeFile(sockfd, serv_addr, argv[2]);
        break;
      case 'r':
        readFile(sockfd, serv_addr, argv[2]);
        break;
      default: 
        printf("Not a valid action \n");
        break;
    }

  }
  //ready for packet magic here!

}
