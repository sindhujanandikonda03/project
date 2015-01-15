from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
from os import curdir, sep

class MyHandler (BaseHTTPRequestHandler):

        def  do_GET(self):
                 try:
                        if self.path.endswith("stuff.txt"):
                                print curdir + sep + self.path
                                self.send_response(200)

                                self.send_header('Content-type', 'text-html')
                                self.end_headers()

                                print self.command + self.path
                                f = open(curdir + sep + self.path)
                                #self.wfile.write(f.read())
                                self.wfile.write("ABCDEF\n123456\n!@#$%^\n")
                                f.close()
                                return
                        else:
                                print "2nd request"
                                print self.command + self.path
                                print self.headers.items()
                                return
                 except IOError:
                         self.send_error("file not found.")

#def main():
try:
        server = HTTPServer (('', 80), MyHandler)
        print 'start server'
        #server.serve_forever()
        while 1:
                server.handle_request()
except KeyboardInterrupt:
        server.socket.close()

