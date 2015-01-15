import socket
import binascii
from Crypto.Hash import MD5
from Crypto.Cipher import AES

TCP_IP = '192.168.14.10'
TCP_PORT = 3008
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

#s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
#s.connect ((TCP_IP, TCP_PORT))
#s.send(MESSAGE)
#data = s.recv (BUFFER_SIZE)
#s.close()

#print "received:", data

s1 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
#s1.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s1.bind(('0.0.0.0', 3808))
s1.listen(1)

while 1:
        conn, addr = s1.accept()
        data = conn.recv(BUFFER_SIZE)
        if not data: break
        print "received:", data
        s2 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
        s2.connect ((TCP_IP, TCP_PORT))
        s2.send(data)
        data1 = s2.recv(BUFFER_SIZE)
        print "received from server:", data1
        conn.sendall(data1)
        data2 = conn.recv(BUFFER_SIZE)
        print "received:", data2
conn.close()

