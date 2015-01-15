import socket
#: FLAG b9fbeb0ec51219e2
UDP_IP = "192.168.14.147"
UDP_PORT =  2001

sock = socket.socket (socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

while True:
        data, addr = sock.recvfrom(1024)
        print "received:", data

