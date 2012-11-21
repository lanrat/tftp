#include "tftp.h"

//used for counting the number of children procs
static unsigned int childCount = 0;

void run_child(struct sockaddr cli_addr, PACKET * packet)
{
  if (packet == NULL)
  {
    return;
  }

  int child_sockfd;
  bool result;

  //create the child socket which will be on a new port
  child_sockfd = createUDPSocketAndBind(0);

  switch (packet->optcode)
  {
    case TFTP_OPTCODE_RRQ:
      //TODO handle some stuff
      //make a call to sendfile()
      break;
    case TFTP_OPTCODE_WRQ:
      //TODO handle some other things
      //make a call to recvfile()
      break;
    default:
      result = send_error(child_sockfd, &cli_addr, TFTP_ERRCODE_ILLEGAL_OPERATION,"Unexpected Packet");
      break;
  }

  //TODO this will need to be moved for RRQ and WRQ
  if (result == false)
  {
    printf("Failed sending msg to client\n");
  }

}


void packet_recieve_loop(int sockfd)
{
  struct sockaddr cli_addr;
  size_t cli_size = sizeof(cli_size);
  int recv_len;
  char buffer[BUFSIZE];
  pid_t fork_id;
  PACKET packet;

  //main loop
  while (true)
  {
    //clean the buffer
    memset(buffer,0,BUFSIZE);

    //recieve the data
    recv_len = recvfrom(sockfd, buffer, BUFSIZE, 0, &cli_addr, (socklen_t *)&cli_size);

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
      unserializePacket(buffer,recv_len,&packet);
      fork_id = fork();
    }else{
      printf("Error: Server too busy to accept new clients\n");
      send_error(sockfd, &cli_addr, TFTP_ERRCODE_UNDEFINED,"Too many clients");
      continue;
    }

    if (fork_id < 0)
    {
      printf("Error: could not fork child process\n");
      send_error(sockfd, &cli_addr, TFTP_ERRCODE_UNDEFINED,"Unable to handle client request");
      exit(1);
    }else if (fork_id == 0)
    {
      //close the parents Socket, we will make our own
      close(sockfd);
      childCount++;

      if (DEBUG) printPacket(&packet);

      //child code here, deal with the packet
      run_child(cli_addr,&packet);

      //end of child execution
      childCount--;
      exit(0); //end the child
    }else
    {
      //main server code
      if (DEBUG) printf("spawning fork: %u to handle new request\n",fork_id);
    }

  }
  close(sockfd);
}


int main(int argc, char *argv[])
{
  //used for error messages
  progname = argv[0];

  //TODO allow changing of default port via comand line args

  int sockfd;

  //bind to our socket
  sockfd = createUDPSocketAndBind(SERV_UDP_PORT);

  //run the main loop
  packet_recieve_loop(sockfd);

  return 0;
}

