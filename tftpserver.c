#include "tftp.h"
#include "packets.h"
#include "test.h"

void packet_recieve_loop(int sockfd)
{
  struct sockaddr pcli_addr;
  int client_len = sizeof(pcli_addr);
  int recv_len;
  char buffer[BUFSISE];

  //main loop
  while (true)
  {
    recv_len = recvfrom(sockfd, buffer, BUFSISE, 0, &pcli_addr, &client_len);

    //check for errors
    if (recv_len <= 0)
    {
      printf("%s: recvfrom error\n",progname);
      exit(1);
    }
    //TODO
    /* Note that if you are using timeouts, n<0 may not mean an error, */
    /* but that the call was interrupted by a signal. To see what      */
    /* happened, you have to look at the value of the system variable  */
    /* errno (defined in <errno.h>).*/

    PACKET * packet = getPacket(buffer);

    if (packet != NULL)
    {
      printPacket(packet);

    }else{
      printf("Unable to determine packet type\n");
    }
  }
}


int main(int argc, char *argv[])
{
  //used for error messages
  progname = argv[0];

  //create a socket object
  struct sockaddr_in serv_addr;
  int sockfd;
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    printf("%s: can't open datagram socket\n",progname);
    exit(1);
  }
  
  //zero out the struct
  bzero((char*) &serv_addr, sizeof(serv_addr));

  //set some struct args
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(SERV_UDP_PORT);

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("%s: can't bind local address\n",progname);
    exit(1);
  }

  //run the main loop
  packet_recieve_loop(sockfd);

}

