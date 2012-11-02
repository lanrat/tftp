CXX      = gcc

default: all

all: server client

server:
	${CXX} -o tftpserver tftpserver.c


client: 
	${CXX} -o tftpclient tftpcient.c

clean:
	rm *.o tftpclient tftpserver
