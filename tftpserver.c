#include "tftp.h"
#include "packets.h"
#include "test.h"

//used for counting the number of children procs
static unsigned int childCount = 0;

void packet_recieve_loop(int sockfd)
{
  struct sockaddr pcli_addr;
  int client_len = sizeof(pcli_addr);
  int recv_len;
  char buffer[BUFSIZE];
  pid_t fork_id;
  PACKET * packet;

  //main loop
  while (true)
  {
    //clean the buffer
    memset(buffer,0,BUFSIZE);

    //recieve the data
    recv_len = recvfrom(sockfd, buffer, BUFSIZE, 0, &pcli_addr, &client_len);

    //check for errors
    if (recv_len <= 0)
    {
      printf("%s: recvfrom error\n",progname);
      perror("Socket read:");
      if (recv_len == 0)
      {
        printf("Socket closed\n");
      }
      exit(1);
    }
    //TODO
    /* Note that if you are using timeouts, n<0 may not mean an error, */
    /* but that the call was interrupted by a signal. To see what      */
    /* happened, you have to look at the value of the system variable  */
    /* errno (defined in <errno.h>).*/

    if (childCount < MAX_TFTP_CLIENTS)
    {
      packet = getPacket(buffer);
      fork_id = fork();
    }else{
      //TODO send error
      printf("Error: Server too busy to accept new clients\n");
      continue;
    }

    if (fork_id < 0)
    {
      printf("Error: could not fork child process\n");
      exit(1);
    }else if (fork_id == 0)
    {
      //close the parents Socket, we will make our own
      close(sockfd);
      childCount++;

      if (DEBUG) printPacket(packet);
      
      //child code here, deal with the packet
      
      free(packet);
      
      //end of child execution
      childCount--;
      exit(0); //end the child
    }else
    {
      //main server code
      if (DEBUG) printf("spawning new fork with id %u to handle incomming request\n",fork_id);
    }

  }
  close(sockfd);
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

  return 0;
}

