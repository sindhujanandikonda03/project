import httplib

#conn = httplib.HTTPConnection('192.168.14.50', 80)
conn = httplib.HTTPConnection('192.168.14.50', 80)
conn.connect()

conn.request('GET', '/lab05/prob01/')
#request = conn.putrequest('POST', '/lab05/prob01/')

resp = conn.getresponse()
#print resp.status
lines = resp.getheaders()
for line in lines:
        item1, item2 = line
        if item2.find('FLAG')>=0:
                print item2
#print resp.read()

conn.close()

