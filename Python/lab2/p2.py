import socket
#received: FLAG bb859cc3ad0cf0b9

UDP_IP = "192.168.14.10"
RCV_IP = "0.0.0.0"
UDP_PORT = 2002
MESSAGE = "GET FLAG"

sock = socket.socket (socket.AF_INET, socket.SOCK_DGRAM)

sock1 = socket.socket (socket.AF_INET, socket.SOCK_DGRAM)

sock.sendto (MESSAGE, (UDP_IP, UDP_PORT))

sock1.bind((RCV_IP, UDP_PORT))

while True:
        data, addr = sock1.recvfrom(1024)
        print "received:", data

