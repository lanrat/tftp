CXX = gcc

common = packets.c test.c common.c pong.c
flags = -g

default: all

all: server client

server:
	${CXX} ${flags} -o tftpserver ${common} tftpserver.c 

client: 
	${CXX} ${flags} -o tftpclient ${common} tftpclient.c

clean:
	rm *.o tftpclient tftpserver
