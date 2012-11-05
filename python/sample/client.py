import socket
import config

MESSAGE = "Hello World abcd"

print "UDP target IP:", config.UDP_IP
print "UDP target port:", config.UDP_PORT
print "message:", MESSAGE

sock = socket.socket(socket.AF_INET, # Internet
                         socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE, (config.UDP_IP, config.UDP_PORT))

print "Done"
