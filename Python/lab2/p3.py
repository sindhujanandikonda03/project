import socket

#Connect to: ('192.168.14.251', 36609)
#received: FLAG 3a41296749a137d7
#done.

TCP_IP = '192.168.14.147'
TCP_PORT = 2003
BUFFER_SIZE = 1024

s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

conn, addr = s.accept()

print 'Connect to:', addr
while 1:
        data = conn.recv(BUFFER_SIZE)
        if not data: break
        print "received:", data
        conn.send(data)
conn.close()

