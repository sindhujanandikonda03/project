import socket
import binascii
from Crypto.Hash import MD5
from Crypto.Cipher import AES

TCP_IP = '192.168.14.10'
TCP_PORT = 3007
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s.connect ((TCP_IP, TCP_PORT))
s.send(MESSAGE)
data = s.recv (BUFFER_SIZE)

#print data

IV = data[:16]

with open("/usr/share/dict/words") as file:
        for line in file:
                #print line.lower()
                hash = MD5.new()
                hash.update(line.lower().strip('\r\n').strip())
                key = hash.digest()
                #print "MD5:", key
                decobj = AES.new(key, AES.MODE_CBC, IV)
                plaintext = decobj.decrypt(data[16:])
                #print plaintext
                if plaintext.find("FLAG") >= 0:
                        print "Password Found: ", line
                        print plaintext
                        break
                if not line: break

file.close()

s.close()

