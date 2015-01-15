import socket

TCP_IP = '192.168.14.50'
#open port 23387

BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

try:
        for port_num in range (20000, 30000):
                s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
                result = s.connect_ex ((TCP_IP, port_num))
                if result == 0:
                        print "Port {}: \t Open".format(port_num)
                        print s.recv(1024)
                        x = raw_input('Answer: ')
                        s.send(x)
                        print s.recv(1024)
                        s.send(MESSAGE )
                        break
                s.close()
except Exception as inst:
        print inst.args
while 1:
        data = s.recv(BUFFER_SIZE)
        if not data: break
        print "received:", data
s.close()

