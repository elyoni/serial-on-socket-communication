#!/usr/bin/python
import socket
import _thread
import traceback
import select
import sys # used to check what platform i am using
import platform
import serial

import time


DEBUG = True

class ServerSocket(object):
    def __init__(self,host='127.0.0.1', port=50000, max_connection=5):
        self.ser = serial.Serial('/dev/ttyUSB1', 115200, timeout=1)
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)# Create the Server Socket
        try:
            print('Socket Server is on')
            print('<<<<Press any key to exit>>>>')
            server_address = (host, port)
            self.server.bind(server_address)
            self.server.listen(max_connection)

            # Config inputs
            self.input = [self.server]
            if platform.system() == 'Linux':
                self.input.append(sys.stdin) #not working on windows
            elif platform.system() == 'Win':
                print('The system is windows, input from keyboard is not supported')
            
            # Sockets to which we expect to write
            self.output = []
        except Exception as e:
            print('Problem with creating the Server', e)
    
    def send(self, connection, data):
        connection.send(data.encode('ascii'))

    def receive(self, connection):
        data = connection.recv(1024)
        if data:
            if (data.decode('ascii') == "?id"):
                print("got ?id")
                self.ser.write(b'?id')

                print("print lines", self.ser.read())
            print(data.decode('ascii'))
            self.send(connection, "hi you are connected")
        else:
            #print("Connection has been diconnected")
            self.input.remove(connection)
            connection.close()

    def execute_server(self):
        exit = False
        while not exit:
            try:
                input_ready, output_ready, except_ready = select.select(self.input , self.output, [])
                for connection in input_ready:  # work on all the input that send something
                    if connection is self.server:
                        if DEBUG:
                            print("New connection")
                        connection, client_address = connection.accept()
                        connection.setblocking(0)
                        self.input.append(connection)
                    if connection is sys.stdin:
                        self.server.close()
                        exit = True
                        break
                    else:
                        self.receive(connection)
            except: 
                self.server.close()
                raise

        print("The server is close")

if __name__ == "__main__":
    server_socket = ServerSocket()
    server_socket.execute_server()
