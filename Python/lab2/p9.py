import socket

SMTP_IP = '192.168.14.48'
TCP_IP = '192.168.14.147'
TCP_PORT = 2007
BUFFER_SIZE = 1024

s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s.connect ((SMTP_IP, 25))

s1 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s1.bind((TCP_IP, TCP_PORT))
s1.listen(1)

print s.recv(1024)
s.send("mail from: <bob@example.com>\n")
print s.recv(1024)
s.send("rcpt to: alice\n")
print s.recv(1024)

s.send("data\n")
print s.recv(1024)
s.send("From: bob@example.com\nSubject: FLAG\n\nGOTO 192.168.14.147 2007 \n.\n")
print s.recv(1024)
s.close()

conn, addr = s1.accept()

print 'Connect to:', addr
while 1:
        data = conn.recv(BUFFER_SIZE)
        if not data: break
        print "received:", data
        conn.send(data)
conn.close()

