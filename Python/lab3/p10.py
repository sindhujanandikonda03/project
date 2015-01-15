import socket
import binascii
from Crypto.Hash import MD5
from Crypto.Cipher import AES

TCP_IP = '192.168.14.10'
TCP_PORT = 3010
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

#s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
#s.connect ((TCP_IP, TCP_PORT))
#s.send(MESSAGE)
#data = s.recv (BUFFER_SIZE)
#s.close()

#print "received:", data

s1 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s1.bind(('0.0.0.0', 3110))
s1.listen(1)

conn, addr = s1.accept()
print 'Connection from:', addr

while 1:
        data = conn.recv(BUFFER_SIZE).strip()
        if not data: break
        print "received:", data.encode('hex')
        print len(data)
        conn.send(data)

        print data[0].encode('hex')
        print data[41].encode('hex')

        #newdata = data[:44] + chr(ord(data[44]) ^ 0x46) + chr(ord(data[45]) ^ 0x4c) + chr(ord(data[46]) ^ 0x41) + chr(ord(data[47]) ^ 0x47) + data[48:]
        newdata = data[:25] + chr(ord(data[25]) ^ ord('R') ^ 0x4C)  + chr(ord(data[26]) ^ ord('O') ^ 0x41) + data[27:]


        s1 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
        s1.connect ((TCP_IP, TCP_PORT))
        #print data
        s1.send(newdata)
        data1 = s1.recv(BUFFER_SIZE)
        print "received from server:", data1
        #print "received from server:", data1.encode('hex')
conn.close()


