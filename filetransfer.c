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

  u_int16_t blockNumber = 1;
  char buffer[MAX_DATA_SIZE];
  size_t n;
  int result;
  PACKET packet;
  unsigned int timeout_counter;

  do
  {
    n = fread(buffer,1,MAX_DATA_SIZE,fileh);

    if (DEBUG) printf("Sending Data: [%u] ",blockNumber);
    if (!send_data(sockfd, cli_addr, blockNumber,buffer,n))
    {
      return false;
    }
    if (DEBUG) printf("Sent\n");

    //wait for ack
    result = waitForPacket(sockfd, cli_addr, TFTP_OPTCODE_ACK,&packet);

    //check for timeout
    if (result == -1)
    {
      if (timeout_counter > MAX_TFTP_TIMEOUTS)
      {
        send_error(sockfd,cli_addr,TFTP_ERRCODE_ILLEGAL_OPERATION,"Too many timeouts");
        return false;
      }
      timeout_counter++;
    }else
    {
      //check for error
      if (packet.optcode == TFTP_OPTCODE_ERR)
      {
        //call the error handler
        printError(&packet);
        return false;
      }
      //check the ack recieved was for the correct blcok number
      if (packet.ack.blockNumber != blockNumber)
      {
        send_error(sockfd,cli_addr,TFTP_ERRCODE_UNKNOWN_TRANSFER_ID,"Incorect Block Number");
        return false;
      }

      blockNumber++;
      timeout_counter = 0;
    }
  } while (n == MAX_DATA_SIZE);

  return true;
}


/* This function recieves a file from a remote host client or server */
bool recvFile(int sockfd, struct sockaddr* cli_addr, FILE* fileh)
{
  
  PACKET packet;
  int result;
  unsigned int timeout_counter = 0;
  unsigned int expected_block_number = 1;

  do{
      result = waitForPacket(sockfd, cli_addr, TFTP_OPTCODE_DATA, &packet);
      if(result == -1)     //timeout
      {
        if(timeout_counter < MAX_TFTP_TIMEOUTS)
        {
          timeout_counter++;
        }
        else
        {
          send_error(sockfd, cli_addr, 0, "Too many timeouts\n");
          return false; 
        }

      }
      else if(packet.optcode == TFTP_OPTCODE_ERR)
      {
        //error handler
        printError(&packet);
        return false;
      }
      else //correct packet
      {
        //check for correct blocknumber
        //A: Correct
        //if (DEBUG) printPacket(&packet);
        if(packet.data.blockNumber == expected_block_number)
        {
          if(fwrite(packet.data.data, 1, packet.data.dataSize, fileh))
          {
            expected_block_number++;
          }
          else if(packet.data.dataSize != 0)
          {
            //TODO check for all file errror cases
            send_error(sockfd, cli_addr, TFTP_ERRCODE_UNDEFINED, "Could not write to file");
            return false;
          }
          if (DEBUG) printf("Sending Ack: [%u] ",packet.data.blockNumber);
          if (!send_ack(sockfd, cli_addr, packet.data.blockNumber))
          {
            return false;
          }
          if (DEBUG) printf("Sent\n");
        }
        else
        {
          //TODO fix this logic
          send_error(sockfd, cli_addr, TFTP_ERRCODE_ILLEGAL_OPERATION, "Wrong block number recieved");
          return false;
        }
      }
  } while(packet.data.dataSize == MAX_DATA_SIZE);
  return true;
}
