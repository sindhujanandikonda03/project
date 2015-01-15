import socket

s1 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s1.connect(('192.168.14.50', 80))

s1.sendall("GET /lab05/prob01/ HTTP/1.0\r\n")
s1.sendall("\r\n")

data = s1.recv(1024).split('\n')
#print data

for line in data:
      if line.find('FLAG=') > 0:
           flag = line.split(':')
           print flag[1]

