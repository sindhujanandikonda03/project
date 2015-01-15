import socket

#user@LabVM:~/lab2$ python p7.py
#GOTO 192.168.14.206:2007

#Goto: 192.168.14.206
#received: FLAG d00d3c34def7eca2


TCP_IP = '192.168.14.10'
TCP_PORT = 2007
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s.connect ((TCP_IP, TCP_PORT))

s.send(MESSAGE)

data = s.recv(BUFFER_SIZE)
print data

while 1:
        if data.find('GOTO') == 0:
                s.close()
                x = raw_input('Goto: ')
                s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
                s.connect((x, TCP_PORT))
                s.send(MESSAGE)

        data = s.recv(BUFFER_SIZE)
        if not data: break
        print "received:", data

s.close()

