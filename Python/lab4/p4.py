import socket
import binascii
from Crypto import Random
from Crypto.Cipher import AES

TCP_IP = '192.168.14.30'
TCP_PORT = 4004
BUFFER_SIZE = 1024

pad = lambda s: s + (16 - len(s) % 16) * chr(16 - len(s) % 16)
unpad = lambda s : s[:-ord(s[-1])]

s1 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s1.connect((TCP_IP, TCP_PORT))

enc = 'cda284a357d2e7f0ae81f88499c29820e50cbb8c3d7b669a6c5784a5667aa09d1669275067ebb1c9314c89fe9383bd966354f01ad82f46b5a0ddf09ecc40599d'

print len(enc)
s1.send('cda284a357d2e7f0ae81f88499c29820e50cbb8c3d7b669a6c5784a5667aa09d1669275067ebb1c9314c89fe9383bd966354f01ad82f46b5a0ddf09ecc40599d')


challenge = s1.recv(BUFFER_SIZE).split()
print challenge

key_bob_charlie = binascii.unhexlify('1482e4982b566028102db2635cc4f936')

#decrypt CHALLENGE

rawbytes_challenge = binascii.unhexlify(challenge[1])
dec_nb = AES.new(key_bob_charlie, AES.MODE_CBC, rawbytes_challenge[:16])
nb = unpad(dec_nb.decrypt(rawbytes_challenge))[16:]
print "Nb: ", nb

#encrypt RESPONSE and send to Charlie
IV = Random.new().read(16)
enc_resp = AES.new(key_bob_charlie, AES.MODE_CBC, IV)

padded_msg = pad(str(int(nb)-1))
print "Nb-1: ", str(int(nb)-1)
print "pad: ", padded_msg.encode("hex")
print len(padded_msg)
response = enc_resp.encrypt(padded_msg)

s1.sendall("RESPONSE "+(IV+response).encode('hex'))
s1.sendall("GET FLAG")

flag = s1.recv(BUFFER_SIZE)
print flag


