import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('localhost', 50000))
msg = 'Thank you for connecting'
s.send(msg.encode('ascii'))
data = s.recv(1024)
s.close()
print(data.decode('ascii'))
