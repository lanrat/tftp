#include "filetransfer.h"

/* how use use these functions
   File handles should be opened and checked before any calls
  
   Client sending file:
    send WRQ
    wait for ack
    sendfile()
   Client recieving file:
    send RRQ
    recvfile()
   Server sending file:
    sendfile()
   Server receiving file:
    send ack
    recvfile()

 */


/* This function sends a file to a remote host client or server */
bool sendFile(int sockfd, struct sockaddr* cli_addr, int fileh)
{
  //sudo-code
  //while there is data left to send:
  //  send a data pacekt
  //  wait for ack, check for error recieved
  //    if timeout:
  //      if timeout_counter > max_timeouts:
  //        send error
  //        return false
  //      else:
  //        increase timeout_counter
  //        resend data
  //    reset timeout_counter
  //return true
  return false;
}


/* This function recieves a file from a remote host client or server */
bool recvFile(int sockfd, struct sockaddr* cli_addr, int fileh)
{
  //sudo code
  //while we recieve more data packets: (check for errors recieved)
  //  if timeout:
  //    if timeout_counter > max_timeouts:
  //      send error
  //      return false
  //    increase timeout_counter
  //    resend ack
  //  else:
  //    reset timeout_counter
  //    write the data to a file
  //    if successful:
  //      send ack
  //    else:
  //     send error
  //     return false
  //return true
  return false;
}
