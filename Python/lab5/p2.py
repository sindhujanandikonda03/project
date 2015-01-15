import socket

s1 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s1.connect(('192.168.14.50', 80))

s1.sendall("GET /lab05/prob02/ HTTP/1.0\r\n")
s1.sendall("\r\n")

data = s1.recv(1024).split('\n')

for line in data:
        if line.find('Set-Cookie')>=0:
                part = line.split(';')
                print part[0]
                cookie = part[0].split(' ')
                print cookie[1]
s1.close()

s2 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s2.connect(('192.168.14.50', 80))
s2.sendall("GET /lab05/prob02/ HTTP/1.0\r\n")
s2.sendall("Cookie: " + cookie[1]+"\r\n")
s2.sendall("\r\n")

data1 = s2.recv(1024).split('\n')
for line1 in data1:
        if line1.find('FLAG')>=0:
                flag = line1.split(' ')
                print "FLAG ", flag[2][:-4]
s2.close()


