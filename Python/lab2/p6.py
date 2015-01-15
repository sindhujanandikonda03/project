import socket
TCP_IP = '192.168.14.40'
TCP_PORT = 2006
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s.connect ((TCP_IP, TCP_PORT))

data = s.recv(BUFFER_SIZE)
print data

x = raw_input('Pass: ')
s.send(x)

while 1:
    data = s.recv(BUFFER_SIZE)
    if not data: break
    print "received:", data
    s.send(MESSAGE)
    data = s.recv(BUFFER_SIZE)
    print "received:", data
s.close()

