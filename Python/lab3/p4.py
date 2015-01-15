import socket
import binascii
from Crypto.Cipher import AES

TCP_IP = '192.168.14.10'
TCP_PORT = 3004
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s.connect ((TCP_IP, TCP_PORT))
s.send(MESSAGE)
data = s.recv (BUFFER_SIZE)

IV = data[:16]

key = binascii.unhexlify('20140304201403042014030420140304')
decobj = AES.new(key, AES.MODE_CBC, IV)
plaintext = decobj.decrypt(data[16:])

print "Decrypt:", plaintext

s.close()

