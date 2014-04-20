#!/usr/bin/env python2

from __future__ import print_function
from socket import *
import os

def warn(*objs):
    print("ERROR: ", *objs, file=sys.stderr)

serverName = os.environ['HOSTNAME']
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_STREAM)
try:
    clientSocket.connect((serverName,serverPort))
except:
    warn("Failed to connect to server")
    exit(1)
message = raw_input('Input lowercase sentence: ')
try:
    clientSocket.send(message)
except:
    warn("Failed to send message")
try:
    modifiedMessage = clientSocket.recv(1024)
except:
    warn("Failed to receive message")
    modifiedMessage = ""
print ( modifiedMessage )
clientSocket.close()
