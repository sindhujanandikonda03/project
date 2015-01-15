import socket
import binascii
from Crypto.Cipher import AES

TCP_IP = '192.168.14.10'
TCP_PORT = 3003
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

s = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s.connect ((TCP_IP, TCP_PORT))
s.send(MESSAGE)
data = s.recv (BUFFER_SIZE)

key = binascii.unhexlify('20140303201403032014030320140303')
decobj = AES.new(key, AES.MODE_ECB)
plaintext = decobj.decrypt(data)

print "Decrypt:", plaintext

s.close()

