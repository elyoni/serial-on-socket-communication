#!/usr/bin/python
import socket
import _thread
import traceback
import select
import os
import sys # used to check what platform i am using
import platform
import serial

import time

DEBUG = True
serial_connection = '/dev/ttyUSB1'

def print_debug(string_to_print):
    if DEBUG:
        print(string_to_print)

class ServerSocket(object):
    """ The class is connection a socket connetion to serial connection """
    def __init__(self,host='127.0.0.1', port=50000, max_connection=5):
        try:
            self.ser = serial.Serial(serial_connection, 115200, timeout=1)
            self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)# Create the Server Socket
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
                print_debug('The system is windows, input from keyboard is not supported')
            
            # Sockets to which we expect to write
            self.output = []
        except FileNotFoundError as e:
            print("There is no", serial_connection, "The server cannot start running, make sure the device is connected")
            raise
        except serial.serialutil.SerialException as e:
            print(e)
            raise
        except Exception as e:
            print('Problem with creating the Server', e)
            exc_type, exc_obj, exc_tb = sys.exc_info()
            print("[" + os.path.basename(__file__) + "]", "Exception:", e,
                  exc_type, " error line:", exc_tb.tb_lineno)

            raise
    
    def send(self, connection, data):
        connection.send(data.encode('ascii'))

    def receive(self, connection):
        """ The function collect the sokect msg and send it to the serial device """
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
        """ Handeling the message/connection that is comming from the socket connection """
        exit = False
        while not exit:
            try:
                input_ready, output_ready, except_ready = select.select(self.input , self.output, [])
                for connection in input_ready:  # work on all the input that send something
                    if connection is self.server:
                        print_debug("New connection")
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
    try:
        server_socket = ServerSocket()
        server_socket.execute_server()
    except:
        print("ERROR, The program will stop")
