#!/usr/bin/env python3

import socket
import json
import sys

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
        hostname = sys.argv[1]
        port = sys.argv[2]
except:
        print("Missing arguments. Closing.")
        exit()

try:
        s.connect((hostname,int(port)))
except EnvironmentError as exc:
        print("Cannot create connection. Closing.")
        s.close()
        exit()

uinput = ''

rcvdData = s.recv(1024).decode()
response = json.loads(rcvdData)
print(response)
uinput = input("> ")
sendData = uinput
sendData = json.dumps(sendData)
s.send(sendData.encode())
rcvdData = s.recv(1024).decode()
response = json.loads(rcvdData)
print(response)
uinput = input("> ")
sendData = uinput
sendData = json.dumps(sendData)
s.send(sendData.encode())

while uinput != '1':
    rcvdData = s.recv(1024).decode()
    data = []
    try:
        response = json.loads(rcvdData)
    except:
        print(rcvdData[-1])
        print("exception")
        continue
    #if "The contest is over - thanks for playing" in response:
    #    print(response)
    #    break
    if type(response) is list:
        print("Question ", response[0], ": ")
        print(response[1][1])
        for attributes in response[1][2]:
            print(attributes)

        uinput = input("Enter Choice: ")
        if uinput == response[1][3]:
            sendData = "Correct"
            print("You are Correct")
        else:
            sendData = "incorrect"
            print("You are incorrect")
        sendData = json.dumps(sendData)
        s.send(sendData.encode())
    else:
        print(response)
        if type(response) is str:
            if "The contest is over - thanks for playing" in response:
                break
            if "Your score is" in response:
                sendData = " "
                sendData = json.dumps(sendData)
                s.send(sendData.encode())
                continue
            if "Results: " in response:
                sendData = " "
                sendData = json.dumps(sendData)
                s.send(sendData.encode())
                continue
            else:
                uinput = input("> ")
                sendData = uinput
                sendData = json.dumps(sendData)
                s.send(sendData.encode())
