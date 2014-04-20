#!/usr/bin/env python2

from __future__ import print_function
from socket import *
import os

def warn(*objs):
    print("ERROR: ", *objs, file=sys.stderr)

serverName = os.environ['HOSTNAME']
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_DGRAM)
message = raw_input('Input lowercase sentence: ')
try:
    clientSocket.sendto(message, (serverName, serverPort))
except CommError:
    warn("Failed to send message")
try:
    modifiedMessage, serverAddress = clientSocket.recvfrom(2048)
except CommError:
    warn("Failed to receive message")
    modifiedMessage = ""
print ( modifiedMessage )
clientSocket.close()
