import httplib

conn = httplib.HTTPConnection('192.168.14.50', 80)

headers = {'Cookie' : 'counter=1000'}
conn.request('GET', '/lab05/prob04/', None, headers)

resp = conn.getresponse()

lines = resp.read().split('\n')

for line in lines:
       if line.find('FLAG') >=0:
              flag = line.split('=')
              print "FLAG", flag[1][:-4]

conn.close()

