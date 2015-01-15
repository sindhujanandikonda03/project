import socket
from random import getrandbits
from Crypto.Hash import MD5
from Crypto.Cipher import AES
from Crypto import Random


TCP_IP = '192.168.14.40'
TCP_PORT = 4002
BUFFER_SIZE = 1024

g = 2
prime = 999959
bits = 32

priv_key = getrandbits(bits)
print "priv", priv_key
pub_key = pow(g, priv_key, prime)
print "pubkey", pub_key

s1 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s1.bind(('0.0.0.0', 4202))
s1.listen(1)

conn, addr = s1.accept()

while 1:
        pub_key_client = conn.recv(BUFFER_SIZE).split()
        if not pub_key_client: break
        print "pubkey_client:", pub_key_client[1]
        shared_secret_client = pow(long(pub_key_client[1]), priv_key, prime)
        hash = MD5.new()
        hash.update(str(shared_secret_client))
        session_key_client = hash.digest()
        conn.sendall("PUBKEY "+str(pub_key))
        print "send to client:", pub_key

        s2 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
        s2.connect ((TCP_IP, TCP_PORT))
        pub_key_server = s2.recv(BUFFER_SIZE).split()
        print "pubkey_server:", pub_key_server
        print "send to server..."
        s2.send("PUBKEY "+ str(pub_key))

        data = conn.recv(BUFFER_SIZE)
        #print "received from client:", data
        decobj = AES.new(session_key_client, AES.MODE_CBC, data[:16])
        plaintext = decobj.decrypt(data[16:])
        print plaintext

        shared_secret_server = pow(long(pub_key_server[1]), priv_key, prime)
        hash1 = MD5.new()
        hash1.update(str(shared_secret_server))
        session_key_server = hash1.digest()
        IV = Random.new().read(16)
        encobj = AES.new(session_key_server, AES.MODE_CBC, IV)

        ciphertext = encobj.encrypt(plaintext.strip())

        s2.sendall(IV+ciphertext)

        data1 = s2.recv(BUFFER_SIZE)
        #print "data:", data1

        decobj1 = AES.new(session_key_server, AES.MODE_CBC, data1[:16])
        plaintext1 = decobj1.decrypt(data1[16:])
        print plaintext1
conn.close()

