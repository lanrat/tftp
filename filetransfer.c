#include "filetransfer.h"

/* This function sends a file to a remote host client or server */
bool sendfile(int sockfd, int fileh)
{
  //sudo-code
  //while there is data left to send:
  //  send a data pacekt
  //  wait for ack
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
bool recvfile(int sockfd, int fileh)
{
  //sudo code
  //while we recieve more data packets:
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
