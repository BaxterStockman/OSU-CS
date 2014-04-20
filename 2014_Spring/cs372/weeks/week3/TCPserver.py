#!/usr/bin/env python2

from socket import *


serverPort = 12000
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))
serverSocket.listen(True)
print "The server is ready to receive"
while True:
    try:
        connectionSocket, clientAddress = serverSocket.accept()
        modifiedMessage = connectionSocket.recv(1024).upper()
        connectionSocket.send(modifiedMessage)
    except:
        print "Error receiving message"
    connectionSocket.close()
