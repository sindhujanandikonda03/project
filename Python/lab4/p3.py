import socket
import binascii
from Crypto import Random
from Crypto.Cipher import AES

TCP_IP = '192.168.14.40'
TCP_PORT = 4003
BUFFER_SIZE = 1024

pad = lambda s: s + (16 - len(s) % 16) * chr(16 - len(s) % 16)
unpad = lambda s : s[:-ord(s[-1])]


s1 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s1.connect((TCP_IP, TCP_PORT))

s1.send("student bob 1234")
data = s1.recv(BUFFER_SIZE)
#print data

key = binascii.unhexlify('20140403201404032014040320140403')

data1 = binascii.unhexlify(data)


#decrypt message from server
decobj = AES.new(key, AES.MODE_CBC, data1[:16])
#plaintext = decobj.decrypt(data1[16:])
plaintext = unpad(decobj.decrypt(data1))[16:]
#print "unpad: ", p
#print "\n\n"
print plaintext

msg_bob = plaintext.split()
#send to Bob
s2 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s2.connect(('192.168.14.20', 4333))

print "send to bob", msg_bob[3]
print "sssss", msg_bob[3].encode('hex')
#s2.send(binascii.unhexlify(msg_bob[3]))
print "key_ab: ", msg_bob[1]
key_ab = binascii.unhexlify(msg_bob[1])
print "length:", len(msg_bob[3])
s2.send(msg_bob[3])

challenge = s2.recv(BUFFER_SIZE).split()
print "Bob response:", challenge

#decrypt CHALLENGE from Bob
print challenge[1]
print challenge[1][:16]
rawbytes_challenge = binascii.unhexlify(challenge[1])
dec_nb = AES.new(key_ab, AES.MODE_CBC, rawbytes_challenge[:16])
nb = unpad(dec_nb.decrypt(rawbytes_challenge))[16:]
print "Nb: ", nb

#encrypt RESPONSE and send to Bob
IV = Random.new().read(16)
enc_resp = AES.new(key_ab, AES.MODE_CBC, IV)

padded_msg = pad(str(int(nb)-1))
print "Nb-1: ", str(int(nb)-1)
print "pad: ", padded_msg.encode("hex")
print len(padded_msg)
response = enc_resp.encrypt(padded_msg)


s2.sendall("RESPONSE "+(IV+response).encode('hex'))
s2.sendall("GET FLAG")

flag = s2.recv(BUFFER_SIZE)
print flag

s2.close()
s1.close()

