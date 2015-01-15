import socket
#: FLAG 1210863b12a0164c

TCP_IP = '192.168.14.20'
TCP_PORT = 2004
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s.connect ((TCP_IP, TCP_PORT))
s.send(MESSAGE)
data = s.recv (BUFFER_SIZE)
s.close()

print "received:", data

