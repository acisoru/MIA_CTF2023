from http.server import BaseHTTPRequestHandler, HTTPServer


class RequestHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        if self.path.startswith('/flag'):
            index = int(self.path.split('=')[1])
            flag = 'CTF{w1r3sh@rk_enj0y3r}'
            if index < len(flag):
                self.send_response(200)
                self.send_header('Content-type', 'text/plain')
                self.end_headers()
                self.wfile.write(bytes(flag[index], 'utf-8'))
            else:
                self.send_response(400)
                self.send_header('Content-type', 'text/plain')
                self.end_headers()
                self.wfile.write(b'Invalid index')

        else:
            self.send_response(404)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(b'Not found')


def run():
    host = 'localhost'
    port = 8080
    server = HTTPServer((host, port), RequestHandler)
    print('Server started on http://{}:{}'.format(host, port))
    server.serve_forever()


if __name__ == '__main__':
    run()