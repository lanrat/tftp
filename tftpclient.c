#include "tftp.h"




//
int main(int argc, char *argv[])
{
  int sockfd;
  int result;
  PACKET packet;

  struct sockaddr_in      serv_addr;

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family      = AF_INET;

  serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
  serv_addr.sin_port        = htons(SERV_UDP_PORT);

  FILE * file;
  file = fopen(argv[2], "rb");

  if(file == NULL)
  {
    //handle error
    return;
  }
  
  sockfd = createUDPSocketAndBind(0);
  if(sockfd == -1)
  {
    //handle error
    return;
  }
  


  if(argv[1][0] == '-')
  {

    switch(argv[1][1])
    {
      case 'w':
        send_WRQ(sockfd, (struct sockaddr *) &serv_addr, argv[2], TFTP_SUPORTED_MODE);
        result = waitForPacket(sockfd, (struct sockaddr *) &serv_addr, TFTP_OPTCODE_ACK, &packet);
        if(result > 0) sendFile(sockfd, (struct sockaddr *) &serv_addr, file);
        break;
      case 'r':
        send_RRQ(sockfd, (struct sockaddr *) &serv_addr, argv[2], TFTP_SUPORTED_MODE);
        recvFile(sockfd, (struct sockaddr *) &serv_addr, file);
        break;
      default: 
        printf("Not a valid action \n");
        break;
    }

  }
  fclose(file);
  //ready for packet magic here!

}
