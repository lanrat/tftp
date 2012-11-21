#include "tftp.h"
#include "packets.h"
#include "pong.h"


void send_packet(int sockfd, struct sockaddr* pserv_addr)
{
  char[] messge = "Hello World!";
  int n = sizeof(message);
}

//
int main(int argc, char *argv[])
{
  //used for error messages
  progname = argv[0];
  int sockfd;

  
  struct sockaddr_in      cli_addr, serv_addr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    printf("%s: can't open datagram socket\n",progname);
    exit(1);
  }

  //set some struct args
  bzero((char *) &cli_addr, sizeof(cli_addr));
  serv_addr.sin_family      = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
  serv_addr.sin_port        = htons(SERV_UDP_PORT);

  //setup the client struct
  bzero((char *) &cli_addr, sizeof(cli_addr));
  cli_addr.sin_family      = AF_INET;
  cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  cli_addr.sin_port        = htons(0);

  if (bind(sockfd, (struct sockaddr *) &cli_addr, sizeof(cli_addr)) < 0)
  {
     printf("%s: can't bind local address\n",progname);
     exit(1);
  }


  if(argv[1][0] == '-')
  {
    switch(argv[1][1])
    {
      case 'r':
        recvfile(argv[2]);
        break;
      case 'w':
        sendfile(argv[2]);
        break;
      case default: 
        printf("Not a valid action \n");
        break;
    }

  }
  //ready for packet magic here!

}
