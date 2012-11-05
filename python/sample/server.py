import socket
import config

sock = socket.socket(socket.AF_INET, # Internet
                         socket.SOCK_DGRAM) # UDP
sock.bind((config.UDP_IP, config.UDP_PORT))

while True:
  data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
  print "received message:", data

print "Done"
