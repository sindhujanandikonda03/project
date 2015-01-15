import httplib
import base64

username = 'bob'
password = 'password'

conn = httplib.HTTPConnection("192.168.14.50", 80)

authen = base64.encodestring("%s:%s" % (username, password))

headers = {"Authorization" : "Basic %s" % authen}

conn.request("GET", "/lab05/prob05/flag.txt", None, headers)

response = conn.getresponse()

#print response.status
print response.read()

conn.close()

