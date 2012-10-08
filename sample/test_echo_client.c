
//Sample program at client side for echo transmit-receive - CSE-123-UCSD-Fall-2012 




#include        <stdio.h>
#include        <sys/types.h>
#include        <sys/socket.h>
#include        <netinet/in.h>
#include        <arpa/inet.h>







#define SERV_UDP_PORT 12345
#define SERV_HOST_ADDR "127.0.0.1"


/* A pointer to the name of this program for error reporting.      */

char *progname;

/* Size of maximum message to send.                                */

#define MAXLINE 512

/* The dg_cli function reads lines from the terminal, sends them   */
/* to the echo server pointed to by pserv_addr, and prints to the  */
/* terminal the echoed data. The local endpoint is sockfd. The     */
/* function returns when an EOF is seen. Note that the server's    */
/* address in pserv_addr is already initialized. Its size is held  */
/* in servlen, so that the function works with other protocol      */
/* families that have different address sizes.                     */

dg_cli(sockfd, pserv_addr, servlen)
int             sockfd;
struct sockaddr *pserv_addr;
int servlen;
{

/* Various counter and buffer variables. The extra byte in the     */
/* receive buffer is used to add a null to terminate the string,   */
/* as the network routines do not use nulls but I/O functions do.  */

	int     n;
	char    sendline[MAXLINE], recvline[MAXLINE + 1];

/* Main client loop. Terminates on EOF. Get terminal input on the  */ 
/* sendline buffer (up to MAXLINE bytes).                          */ 

	while (fgets(sendline, MAXLINE, stdin) != NULL) {
		
/* Find the string's length to pass it as a parameter to the send  */
/* calls (excluding the null in the end).                          */
		
		n = strlen(sendline); 
		
/* Send data using the sockfd socket, to the server at pserv_addr. */
/* We also pass the send buffer and its size (sendline and n), an  */
/* unused flag byte (0) and the server's address size (servlen).   */
/* The returned number is the number of bytes sent. If it is not n */
/* either an error or an interrupt has occured.                    */
		
		if (sendto(sockfd, sendline, n, 0, pserv_addr, servlen) != n)
			{
			 printf("%s: sendto error on socket\n",progname);
			 exit(3);
			}

/* Read the echoed data from the same socket into the recvline     */
/* buffer (up to MAXLINE bytes). The server's address and its size */
/* are returned, but we do not need them, so we use null pointers. */
/* The return value is the number of bytes received.               */

		n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
		if (n < 0)
			{
			 printf("%s: recvfrom error\n",progname);
			 exit(4);
			}
		
/* The exchanged data is not null terminated, as the string length */
/* is explicitly sent. We need to null terminate the string before */
/* using fputs to output it on the terminal.                       */
		
		recvline[n] = 0;
		fputs(recvline, stdout);
	}
}

/* The main program sets up the local socket for communication     */
/* and the server's address and port (well-known numbers) before   */
/* calling the dg_cli main loop.                                   */

main(argc, argv)
int     argc;
char    *argv[];
{
	int                     sockfd;
	
/* We need to set up two addresses, one for the client and one for */
/* the server.                                                     */
	
	struct sockaddr_in      cli_addr, serv_addr;
	progname = argv[0];

/* Initialize first the server's data with the well-known numbers. */

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family      = AF_INET;
	
/* The system needs a 32 bit integer as an Internet address, so we */
/* use inet_addr to convert the dotted decimal notation to it.     */

	serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	serv_addr.sin_port        = htons(SERV_UDP_PORT);

/* Create the socket for the client side.                          */
	
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	       {
		printf("%s: can't open datagram socket\n",progname);
		exit(1);
	       }

/* Initialize the structure holding the local address data to      */
/* bind to the socket.                                             */

	bzero((char *) &cli_addr, sizeof(cli_addr));
	cli_addr.sin_family      = AF_INET;
	
/* Let the system choose one of its addresses for you. You can     */
/* use a fixed address as for the server.                          */
       
	cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
/* The client can also choose any port for itself (it is not       */
/* well-known). Using 0 here lets the system allocate any free     */
/* port to our program.                                            */


	cli_addr.sin_port        = htons(0);
	
/* The initialized address structure can be now associated with    */
/* the socket we created. Note that we use a different parameter   */
/* to exit() for each type of error, so that shell scripts calling */
/* this program can find out how it was terminated. The number is  */
/* up to you, the only convention is that 0 means success.         */

	if (bind(sockfd, (struct sockaddr *) &cli_addr, sizeof(cli_addr)) < 0)
		{
		 printf("%s: can't bind local address\n",progname);
		 exit(2);
		}

/* Call the main client loop. We need to pass the socket to use    */
/* on the local endpoint, and the server's data that we already    */
/* set up, so that communication can start from the client.        */

	dg_cli(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

/* We return here after the client sees the EOF and terminates.    */
/* We can now release the socket and exit normally.                */

	close(sockfd);
	exit(0);
}























