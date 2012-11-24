//#include "filetransfer.h"
#include "tftp.h"

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
bool sendFile(int sockfd, struct sockaddr* cli_addr, FILE* fileh)
{
  //sudo-code
  //while there is data left to send:
  //  send a data packet
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

  u_int16_t blockNumber = 0;
  char buffer[MAX_DATA_SIZE];
  size_t n;

  do
  {
    n = fread(buffer,1,MAX_DATA_SIZE,fileh);
    send_data(sockfd, cli_addr, blockNumber,buffer,n);
  } while (n == MAX_DATA_SIZE);

  return false;
}


/* This function recieves a file from a remote host client or server */
bool recvFile(int sockfd, struct sockaddr* cli_addr, FILE* fileh)
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
  PACKET packet;
  int op = TFTP_OPTCODE_DATA;
  int isItAGodDamnPacketOrNot;
  int timeout_counter = 0;

  do{
      isItAGodDamnPacketOrNot = waitForPacket(sockfd, cli_addr, op, &packet);
      if(isItAGodDamnPacketOrNot == -1)     //timeout
      {
        if(timeout_counter < MAX_TFTP_TIMEOUTS)
        {
          timeout_counter++;
        }
        else
        {
          //call error handler
          return false; 
        }
                            
      }
      else if(isItAGodDamnPacketOrNot == 0) //error
      {
        //call error handler
        return false;
      }
      else                                  //correct packet
      {
        if(fwrite(packet.data.data, 1, isItAGodDamnPacketOrNot, fileh))
        {
          send_ack(sockfd, cli_addr, packet.data.blockNumber);
        }
        else 
        {
          send_error(sockfd, cli_addr, 0,
              "Was not able to write data in data packet to file.\n");
          return false;
        }
      }
  } while(isItAGodDamnPacketOrNot = MAX_DATA_SIZE);
  //} while(n != 0);

  return true;
}
