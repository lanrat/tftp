#ifndef PONG_H
#define PONG_H

#include "tftp.h"

/* Network communications are nothing but a game of pong.
 * I send a packet, you respond. I send another, miss your resposne,
 * you get a point while I get all angry and timeout and send another
 *   --Ian
 */

bool send_RRQ(int sockfd, struct sockaddr_in* sockInfo, char* filename, char* mode);
bool send_WRQ(int sockfd, struct sockaddr_in* sockInfo, char* filename, char* mode);

bool send_data(int sockfd, struct sockaddr_in* sockInfo, u_int16_t blockNumber, char* data, size_t data_size);

bool send_ack(int sockfd, struct sockaddr_in* sockInfo, u_int16_t blockNumber);

bool send_error(int sockfd, struct sockaddr_in* sockInfo, u_int16_t errorCode, char* error_message);

bool waiting(int sockfd, struct sockaddr_in* sockInfo, u_int16_t expectedOp);

void handler(int sig);

#endif
