import socket
import binascii
from Crypto.Hash import HMAC
import Crypto.Hash.SHA

#FLAG aabbf743c6425421 662201ba5ce639f0599589bc7da8dffc38825a5b

TCP_IP = '192.168.14.10'
TCP_PORT = 3006
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

while 1:
        s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
        s.connect ((TCP_IP, TCP_PORT))
        s.send(MESSAGE)
        data = s.recv (BUFFER_SIZE)

        print data

        key = binascii.unhexlify("2014030620140306201403062014030620140306")

        hash = HMAC.new(key, data[:21], Crypto.Hash.SHA)
        digest = hash.hexdigest()
        print "Digest: ", digest
        print "Hash: ", data[22:]
        if digest == data[22:].strip():
                print "\nReal Flag:\n", data
                break
        s.close()

