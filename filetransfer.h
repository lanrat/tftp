#include "tftp.h"


bool sendFile(int sockfd,struct sockaddr* cli_addr, int fileh);

bool recvFile(int sockfd, struct sockaddr* cli_addr,int fileh);
