#include "tftp.h"


int main(int argc, char *argv[])
{
  //used for error messages
  progname = argv[0];

  
  struct sockaddr_in      cli_addr, serv_addr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("%s: can't open datagram socket\n",progname);A
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


  //ready for packet magic here!

}
