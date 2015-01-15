import socket
import binascii
import Crypto.Hash.SHA256

TCP_IP = '192.168.14.10'
TCP_PORT = 3005
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

while 1:
        s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
        s.connect ((TCP_IP, TCP_PORT))
        s.send(MESSAGE)
        data = s.recv (BUFFER_SIZE)

        print data

        sha256 = Crypto.Hash.SHA256.new()

        sha256.update(data[:21])
        digest = sha256.hexdigest()
        print "\n Digest: ", digest
        print "Hash: ", data[22:]
        if digest == data[22:].strip():
                print "\nReal Flag:\n", data
                break
        s.close()

