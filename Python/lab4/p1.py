import socket
from random import getrandbits
from Crypto.Hash import MD5
from Crypto.Cipher import AES
from Crypto import Random


TCP_IP = '192.168.14.40'
TCP_PORT = 4001
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG\x08\x08\x08\x08\x08\x08\x08\x08"

g = 2
prime = 999959
bits = 32

priv_key = getrandbits(bits)
print "priv", priv_key
pub_key = pow(g, priv_key, prime)

s1 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s1.connect((TCP_IP, TCP_PORT))

hash = MD5.new()

data = s1.recv(BUFFER_SIZE).split()
print data[0], data[1]

s1.send("PUBKEY " + str(pub_key))
print "PUBKEY " + str(pub_key)

shared_secret = pow(long(data[1]), priv_key, prime)

print "Share:", shared_secret

hash.update(str(shared_secret))

session_key = hash.digest()

print session_key.encode("hex")

IV = Random.new().read(16)
encobj = AES.new(session_key, AES.MODE_CBC, IV)

ciphertext = encobj.encrypt(MESSAGE)

s1.sendall(IV+ciphertext)

data1 = s1.recv(BUFFER_SIZE)
#print data1

decobj = AES.new(session_key, AES.MODE_CBC, data1[:16])
plaintext = decobj.decrypt(data1[16:])
print plaintext

