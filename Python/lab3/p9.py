import socket
import binascii
from Crypto.Hash import MD5
import struct
import md5py

TCP_IP = '192.168.14.10'
TCP_PORT = 3009
BUFFER_SIZE = 1024
MESSAGE = "GET FLAG"

s1 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
s1.bind(('192.168.14.147', 3909))
s1.listen(1)

conn, addr = s1.accept()
print 'Connection from:', addr

while 1:
        data = conn.recv(BUFFER_SIZE)
        if not data: break
        print "received:", data
        print "Msg:", data[:-36]
        conn.send(data)
        s1 = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
        s1.connect ((TCP_IP, TCP_PORT))

        print data[-35:]

        orgin_md5= data[-35:].strip()
        hash1 = md5py.md5()
        hash1.update("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")

        print orgin_md5[:8]
        print orgin_md5[8:16]
        print orgin_md5[16:24]
        print orgin_md5[24:32]

        p1 = struct.pack("<Q", int (orgin_md5[:8], 16)).replace("\x00", '')
        hash1.A = int(p1.encode("hex"), 16)

        p2 = struct.pack("<Q", int (orgin_md5[8:16], 16)).replace("\x00", '')
        hash1.B = int(p2.encode("hex"), 16)

        p3 = struct.pack("<Q", int (orgin_md5[16:24], 16)).replace("\x00", '')
        hash1.C = int(p3.encode("hex"), 16)

        p4 = struct.pack("<Q", int (orgin_md5[24:32], 16)).replace("\x00", '')
        hash1.D = int(p4.encode("hex"), 16)

        hash1.update(" FLAG")

        print "New MD5: ", hash1.hexdigest()

        pad_str = '\x80'
        pad_len = 64 - 8 - 8 - len(data[:-36])
        print pad_lentho
        for i in range (pad_len - 1):
                pad_str = pad_str + '\x00'
        length = (len(data[:-36]) + 8) *8
        littleE_length = struct.pack('<Q', int(hex(length), 16))
        pad_str = pad_str +  littleE_length
        print pad_str.encode("hex")
        print 'Total length:', len(pad_str) + 8 + len(data[:-36])
        print len(pad_str)

        request = data[:-36] + pad_str + " FLAG" + ' ' + hash1.hexdigest()
        #request = "FLAG " + hash1.hexdigest()
        print "New Request:", request

        s1.send(request)
        data1 = s1.recv(BUFFER_SIZE)
        print "received from server:", data1
conn.close()


