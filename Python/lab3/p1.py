import socket
import binascii

#: FLAG 1210863b12a0164c

TCP_IP = '192.168.14.10'
TCP_PORT = 3001
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s.connect ((TCP_IP, TCP_PORT))
s.send(MESSAGE)
data = s.recv (BUFFER_SIZE).split()

print "received:", data

print binascii.hexlify(data[2])

s.close()

