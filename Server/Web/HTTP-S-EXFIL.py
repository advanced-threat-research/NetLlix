from http.server import BaseHTTPRequestHandler, HTTPServer
import logging
import sys
import ssl

class S(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b'Hello, world!')
 
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        body = self.rfile.read(content_length)
        self.send_response(200)
        self.end_headers()
        decoded = body.decode('utf-8')
        print('[+] Received: ')
        print(decoded)
        f = open('exfil.txt', 'w')
        f.write(decoded)

def runhttps(server_class=HTTPServer, handler_class=S, port=443):
    logging.basicConfig(level=logging.INFO)
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    httpd.socket = ssl.wrap_socket(httpd.socket,
                               server_side=True,
                               certfile="server.pem",
                               keyfile="key.pem",
                               ssl_version=ssl.PROTOCOL_TLS)
    logging.info('Starting secure httpd...\n')
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    logging.info('Stopping secure httpd...\n')

def runhttp(server_class=HTTPServer, handler_class=S, port=80):
    logging.basicConfig(level=logging.INFO)
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    logging.info('Starting httpd...\n')
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    logging.info('Stopping httpd...\n')

if __name__ == "__main__":
    from sys import argv

    if len(argv) == 2 and sys.argv[1] == 'ssl':
        runhttps()
    else:
        runhttp()
