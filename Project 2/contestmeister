#!/usr/bin/env python3
#Code based on https://stackoverflow.com/questions/21233340/sending-string-via-socket-python and Allison Kahn's original file
import socket
import json
import sys
import os
import re

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
qtype = ''
questionDict = {}

#s.send(json.dumps("A " + contestname).encode())
#print(json.loads(s.recv(1024).decode()))


filename = None
try:
	filename = sys.argv[3]
except:
	filename = None
else:
	if not os.path.isfile(filename):
		print("File {} does not exist.".format(filename))
		sys.exit()
	fp = open(filename)
	templist = list(fp)
	CommandList = []
	for commands in templist:
		CommandList.append(commands[:-1])
	fp.close()

linenumber = 0
def getNextCommand():
	global linenumber
	global CommandList
	if(linenumber < len(CommandList)):
		linenumber = linenumber+1
		return CommandList[linenumber-1]
	else:
		global filename
		filename = None
		return None

def adjustedInput(stringvalue):
	if filename == None:
		tempstring = input(stringvalue)
		return tempstring
	else:
		tempstring = getNextCommand()
		if tempstring == None:
			tempstring = input(stringvalue)
			return tempstring
		print(tempstring)
		return tempstring


while qtype != '1':

	qtype = adjustedInput("> ")

	qtypefirst = qtype[:1]
	qtyperest = qtype[1:]

	if qtypefirst == 'p':
		qtyperest = qtyperest.replace(" ", "")

		if qtyperest == "":
			print("Please enter a valid number")
			continue
		if re.search('\d', qtyperest) == None:
			print("Please enter a valid number")
			continue

		tags = ""
		tags = adjustedInput("")
		#tags = tags.split(",")
		qtemp = ""
		question = ""
		while(qtemp != "."):
			qtemp = adjustedInput("")
			question = question + qtemp
		question = question[:-1]
		listOfQ = []
		prevAnswer = "Temp"
		currAnswer = ""
		currAnswerTemp = " "
		counter = 0
		while counter < 1:
			while counter < 1:
				currAnswerTemp = adjustedInput("")
				currAnswer = currAnswer + currAnswerTemp
				if currAnswer == ".":
					counter = counter + 1
				if(currAnswerTemp == "."):
					break
			currAnswer = currAnswer[:-1]
			if(currAnswer != ""):
				if counter == 0:
					counter = 0
				else:
					counter = counter - 1
				listOfQ.append(currAnswer)
				currAnswer = ""
			if len(listOfQ) < 2 and counter ==  1:
				print("Must have at least 2 choices. Please enter another option.")
				counter = counter - 1
			currAnswerTemp = ""
		correctAnswer = adjustedInput("")

		###

		sendData = json.dumps("CHKQUESTION " + qtyperest)
		s.send(sendData.encode())
		rcvdData = s.recv(1024).decode()
		result = json.loads(rcvdData)

		if result == "Error":
			print("Error: Question number " + qtyperest + " is in use.")
			continue

		questionNum = qtyperest
		###

		tempQuestion = {}

		wholeQuestion = []
		wholeQuestion.append(tags)
		wholeQuestion.append(question)
		wholeQuestion.append(listOfQ)
		wholeQuestion.append(correctAnswer)
		tempQuestion[questionNum] = wholeQuestion
		print(questionNum)

		sendData = (tempQuestion)
		sendData = json.dumps(sendData)
		s.send(sendData.encode())
		rcvdData = s.recv(1024).decode()

	elif qtypefirst == 'd':
		sendData = json.dumps(qtype)
		s.send(sendData.encode())
		rcvdData = s.recv(1024).decode()
		response = json.loads(rcvdData)
		print(response)

	elif qtypefirst == 'g':

		sendData = json.dumps(qtype)
		s.send(sendData.encode())
		rcvdData = s.recv(1024).decode()
		response = json.loads(rcvdData)
		if response == '':
			rcvdData = s.recv(1024).decode()
			response = json.loads(rcvdData)
		if response[0:5] == "Error":
			print(response)
		elif type(response) == list:
			print(response[0])
			print(response[1])
			for choice in response[2]:
				print(choice)
			print(response[3])
		else:
			print("Unable to get question")
		"""
		elif qtypefirst == 'r':

		sendData = json.dumps(qtype)
		s.send(sendData.encode())
		rcvdData = s.recv(1024).decode()
		response = json.loads(rcvdData)

		if type(response) != list:
			print("There are no questions entered")
			qtype = "reset"
			continue
		else:
			number, info = response
			print(number)
			print(info[1])
			for x in info[2]:
				print(x)
			useranswer = input("Answer: ")

			sendData = "c " + number + " " +  useranswer

			length = len(info[2])
			charLength = chr(ord('`')+length)
			stop = 0

			if(useranswer <= charLength and useranswer != ""):
				sendData = json.dumps(sendData)
				s.send(sendData.encode())
				rcvdData = s.recv(1024).decode()
				response = json.loads(rcvdData)
				print(response)

			else:
				while stop == 0:
					useranswer = input("That's not an option. Please enter your answer: ")
					if useranswer == "":
						continue
					elif(useranswer <= charLength):
						sendData = "c " + number +" " +  useranswer
						sendData = json.dumps(sendData)
						s.send(sendData.encode())
						rcvdData = s.recv(1024).decode()
						response = json.loads(rcvdData)
						print(response)
						break"""

	elif qtypefirst == 'c':
		qtyperest = qtyperest.replace(" ", "")
		try:
			qnumber = qtyperest[:-1]
			qtrialanswer = qtyperest[-1]
		except:
			print("Error: Please add number and answer.")
			continue
		qtype = "c " + qnumber + " " + qtrialanswer
		sendData = json.dumps(qtype)
		s.send(sendData.encode())
		rcvdData = s.recv(1024).decode()
		response = json.loads(rcvdData)

		print(response)
	elif(qtypefirst == 'k'):
		print("Ending server")
		sendData = "End server"
		sendData = json.dumps(sendData)
		s.send(sendData.encode())

	elif(qtypefirst == 'q'):
		print("Ending client")
		sendData = "Ending client"
		sendData = json.dumps(sendData)
		s.send(sendData.encode())
		break

	elif qtypefirst == 'h':
		helpinfo = "Options: \n\np: put question in the question bank. Each section is separated with a period on a blank line. Enter tags, question text, choices separated by a period each and then an additional period after all choices have been entered and the correct answer. \n\nd: delete a question from the bank. Enter d followed by the number you wish to delete. \n\nr: review a question. Input r <question number> to see the statistics of the specified contest. \n\nc: check answer to a question. Enter c followed by the question number you want to check and then the answer you wish to check in the format c <question number> <answer>. It will return correct or incorrect. \n\nq: terminate the client\n\nk: terminate the server\n\nh: print these instructions\n\ns: s <contest number> This initializes and sets up a contest, use this function before setting up any contests.\n\na: a <Contest Number> <Question Number> A will assign a question from the question bank to the assigned contest. Please make sure that there are questions in the question bank and all contests are initiated (using the s function), otherwise the a function will throw an error.\n\nb: b <contest number> This will begin a contest with the specified contest number. The function will output a port which is only available for 60 seconds. This function allows contestants to connect to the server and compete.\n\nl: l Lists all the contests, whether the contest has been run, and the basic statistics of each contest. Use the r command on a specific contest to get more detailed information. "
		print(helpinfo)

	elif qtypefirst == 'b':
		sendData = qtype
		sendData = json.dumps(sendData)
		s.send(sendData.encode())
		rcvdData = s.recv(1024).decode()
		response = json.loads(rcvdData)
		print(response)

	elif qtypefirst == 's':
		sendData = qtype
		sendData = json.dumps(sendData)
		s.send(sendData.encode())
		rcvdData = s.recv(1024).decode()
		response = json.loads(rcvdData)
		print(response)

	elif qtypefirst == 'a':
		sendData = qtype
		sendData = json.dumps(sendData)
		s.send(sendData.encode())
		rcvdData = s.recv(1024).decode()
		response = json.loads(rcvdData)
		print(response)

	elif qtypefirst == 'l':
		sendData = qtype
		sendData = json.dumps(sendData)
		s.send(sendData.encode())
		rcvdData = s.recv(1024).decode()
		response = json.loads(rcvdData)
		print(response)

	elif qtypefirst == 'r':
		sendData = qtype
		sendData = json.dumps(sendData)
		s.send(sendData.encode())
		rcvdData = s.recv(1024).decode()
		response = json.loads(rcvdData)
		print(response)

	else:
		print("Command not recognized")




s.close()


