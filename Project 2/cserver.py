#!/usr/bin/env python3
#Code based on https://stackoverflow.com/questions/21233340/sending-string-via-socket-python and Allison Kahn's original file
import socket
import json
import random
from _thread import *
import threading
import re
import time

closethread = []
nameOfFile = 'qbank.txt'
contests = []
contestsques = []
contestants = []
contestanswers = []

def countdown(sock, t = 60):
	time.sleep(t)
	global closethread
	for threads in closethread:
		if (threads[0] == sock):
			closethread.remove(threads)
			threads[1] = True
			closethread.append([sock, True])
			break
	return 0

def waitingforplayers(contestnum, questionnum):
	#get number of players
	players = 0
	for contestant in contestants:
		if contestant[0] == contestnum:
			players = players + 1
	answered = 0
	while players != answered:
		answered = 0
		for contestanswer in contestanswers:
			if contestanswer[0] == contestnum:
				if contestanswers[2] == questionnum:
					answered = answered + 1


def save_obj(obj, name):
	#nameOfFile = "qbank.txt"
	obj = json.dumps(obj)
	with open(nameOfFile, 'w') as f:
		json.dump(obj, f, ensure_ascii=False)

def load_obj(name):
	#nameOfFile = "qbank.txt"
	with open(nameOfFile) as json_file:
		file = json.load(json_file)
		return json.loads(file)

def firstmakeConnection():
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	s.bind(('', 0))
	global port
	host = socket.gethostname()
	port  = s.getsockname()[1]
	print(host)
	print(port)
	s.listen(5)
	c, addr = s.accept()
	return c

def makeConnection():
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	s.bind(('', port))
	print(port)
	s.listen(5)
	c, addr = s.accept()
	return c

def newClient(oldc, contestnumber = 1, s = None, Port = None):
	if s == None:
		contestnumber = str(contestnumber)
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		s.bind(('', 0))
		newport = s.getsockname()[1]
		print("Contestant ", contnumber, " started on port ", newport)
		tempsendData = json.dumps(newport)
		oldc.send(tempsendData.encode())
		global closethread
		closethread.append([s, False])
		start_new_thread(countdown, (s, 60))
		s.listen(5)
		try:
			c, addr = s.accept()
			start_new_thread(newClient, ((oldc, contestnumber, s, newport)))
		except:
			print("listening closed")
			return
		else:
			for thread in closethread:
				if s == thread[0]:
					if thread[1] == True:
						print("Closing thread (Passed Timeout).")
						s.close()
						closethread.remove(thread)
						return
					#closethread.remove(thread)
			print("Connected successfully")

	else:
		c, addr = s.accept()
		newport = Port
		for thread in closethread:
			if s == thread[0]:
				if thread[1] == False:
					start_new_thread(newClient, ((oldc, contestnumber, s, newport)))
	sendData = "Please input a nickname:"
	sendData = json.dumps(sendData)
	c.send(sendData.encode())
	nickname = c.recv(1024).decode()
	nickname = json.loads(nickname)
	unique = False
	while unique == False:
		unique = True
		for contestant in contestants:
			if contestant[0] == contestnumber:
				if nickname == contestant[1]:
					sendData = "Sorry, that nickname is already in use\nPlease input a nickname:"
					sendData = json.dumps(sendData)
					c.send(sendData.encode())
					nickname = c.recv(1024).decode()
					nickname = json.loads(nickname)
					unique = False
					break
	contestants.append([contestnumber, nickname, newport, c, addr])

	sendData = "Welcome " + nickname + ", get ready for the contest! (return any key to continue)"
	sendData = json.dumps(sendData)
	c.send(sendData.encode())
	c.recv(1024).decode()
	numofQues = 0
	answeredcorrect = 0
	for questionsarray in contestsques:
		if questionsarray[0] == contestnumber:
			numofQues = numofQues+1
			sendData = questionDict[questionsarray[1]]
			sendData = [questionsarray[1], sendData]
			sendData = json.dumps(sendData)
			c.send(sendData.encode())
			rcvdData = c.recv(1024).decode()
			rcvdData = json.loads(rcvdData)
			global contestanswers
			contestanswers.append([contestnumber, nickname, questionsarray[1], rcvdData])
			#print(contestanswers)
			if (rcvdData == 'Correct'):
				answeredcorrect = answeredcorrect+1
			sendData = "Your score is " + str(answeredcorrect) + "/" + str(numofQues) + ". (Waiting for other player...)"
			sendData = json.dumps(sendData)
			c.send(sendData.encode())
			c.recv(1024).decode()
			#waitingforplayers(contestnum, questionsarray[1])
			# get number of players
			players = 0
			correctans = 0
			for contestant in contestants:
				if contestant[0] == contestnumber:
					players = players + 1
			answered = 0
			while players != answered:
				answered = 0
				correctans = 0
				for contestanswer in contestanswers:
					if contestanswer[0] == contestnumber:
						if contestanswer[2] == questionsarray[1]:
							answered = answered + 1
							if contestanswer[3] == 'Correct':
								correctans = correctans + 1
			#Get top question
			maxques = []
			nicknames = []
			for contestanswer in contestanswers:
				if contestanswer[1] in nicknames:
					continue
				else:
					nicknames.append(contestanswer[1])
			for name in nicknames:
				maxquestionright = 0
				for contestanswer in contestanswers:
					if contestanswer[0] == contestnumber:
						if contestanswer[1] == name:
							if contestanswer[3] == 'Correct':
								maxquestionright = maxquestionright + 1
				maxques.append(maxquestionright)
			topanswer = max(maxques)
			averagecorrectpercent = float(correctans)*100/float(answered)
			sendData = "Results: " + str(averagecorrectpercent) + "% of contestants answered correctly. Top score is " + str(topanswer) + "/"+str(numofQues) + "."
			sendData = json.dumps(sendData)
			c.send(sendData.encode())
			c.recv(1024).decode()
			print(contestnumber + " " + nickname + " has proceeded to the next question.")
	sendData = 'The contest is over - thanks for playing ' + nickname
	sendData = json.dumps(sendData)
	c.send(sendData.encode())


	return newport

c = firstmakeConnection()
questionDict = {}

try:
		questionDict = load_obj("questions")
		if type(questionDict) == str:
			questionDict = {}
except (OSError, IOError, EOFError) as e:
		questionDict = json.dumps({})
		save_obj(questionDict, "questions")
		questionDict = {}


while True:
	rcvdData = c.recv(1024).decode()
	if rcvdData != "":
		temp = json.loads(rcvdData)
		tempfirst = ""
		if type(temp) != dict:
			tempfirst = temp[:1]
			temprest = temp[1:]

		if temp == "Nothing to send":

			sendData = ""

		elif tempfirst == 's':
			temprest = temprest[1:]
			if temprest == "":
				sendData = "Please enter a valid number"
				sendData = json.dumps(sendData)
			elif re.search('\d', temprest) == None:
				sendData = "Please enter a valid number"
				sendData = json.dumps(sendData)

			elif temprest in contests:
				sendData = "Error: Contest number " + temprest + " already exists."
				sendData = json.dumps(sendData)
			else:
				contests.append(temprest)
				sendData = "Contest " + temprest + " is set."
				sendData = json.dumps(sendData)

			"""elif tempfirst == "A":
			temprest = temprest[1:]
			nameOfFile = temprest
			sendData = "File name Changed"
			sendData = json.dumps(sendData)
			#loadquestions()
			print("Using file: " + nameOfFile)
			"""
		elif temp == "End server":
			break
		elif temp == "Ending client":
			c.close()
			c = makeConnection()
			sendData = ""
			sendData = json.dumps(sendData)


		elif "CHKQUESTION" in temp:
			temp = temp.split()
			questionNum = temp[1]
			if questionNum in questionDict:
				sendData = "Error"
			else:
				sendData = "1"
			sendData = json.dumps(sendData)


		elif type(temp) == dict:
			for key, value in temp.items():
				questionDict.update({key:value})
			save_obj(questionDict, "questions")
			sendData = json.dumps("")
		elif temp == "GetDict":

			sendData = json.dumps(questionDict)
		elif tempfirst == "d":
			temprest = temprest.strip(" ")
			if temprest == "":
				sendData = "Please enter a valid number"
			elif re.search('\d', temprest) == None:
				sendData = "Please enter a valid number"

			elif temprest in questionDict:
				sendData = ("Deleted question " + temprest)
				questionDict.pop(temprest)
			else:
				sendData = (temprest +  " is not available to delete")
			sendData  = json.dumps(sendData)
		elif tempfirst == "g":
			temprest = temprest.strip(" ")
			if temprest in questionDict:
				sendData = questionDict[temprest]
			else:
				sendData = ("Error: question " + temprest + " not found")
			sendData = json.dumps(sendData)
			"""elif tempfirst == "r":
			if bool(questionDict) == False:
				sendData = "None"
			else:
				sendData = random.choice(list(questionDict.items()))
			sendData = json.dumps(sendData)"""
		elif tempfirst == "c":
			temprest = temprest[1:]
			qnumber= temprest.split(" ")[0]
			answer = temprest.split(" ")[1]
			if qnumber in questionDict:
				if questionDict[qnumber][3] == answer:
					sendData = "Correct"
				else:
					sendData = "Incorrect"
			else:
				sendData = "Question not entered"
			sendData = json.dumps(sendData)
		elif tempfirst  == 'b':
			try:
				temprest = temprest.strip(" ")
				contnumber = int(temprest)
				#print(contnumber)
			except:
				sendData = "Error: Please enter a valid number for b"
				sendData = json.dumps(sendData)
			else:
				if temprest in contests:
					newport = start_new_thread(newClient, (c, contnumber, None, None))
				else:
					sendData = "Error: Please enter a valid number for b or set Contest number using the s command."
					sendData = json.dumps(sendData)
					c.send(sendData.encode())
				#print("connected successfully")
				#print(contestants)
				continue

		elif tempfirst == 'a':
			temprest = temprest[1:]
			try:
				temprest = temprest.split()
				contestnum = temprest[0]
				questionNum = temprest[1]
			except:
				sendData = "Error: Please enter a valid contest number for a and question number."
				sendData = json.dumps(sendData)
				c.send(sendData.encode())
				continue
			if contestnum not in contests:
				sendData = "Error: contest " + contestnum + " does not exist."
				sendData = json.dumps(sendData)
			elif questionNum not in questionDict:
				sendData = "Error: Question number " + questionNum + " does not exist."
				sendData = json.dumps(sendData)
			else:
				contestsques.append([contestnum, questionNum])
				sendData = "Added Question " + questionNum + " to contest " + contestnum + ". "
				sendData = json.dumps(sendData)

		elif tempfirst == 'l':
			contestsarray = []
			sendData = ""
			for contest in contests:
				#Get questions
				questions = 0
				for question in contestsques:
					if str(contest) == str(question[0]):
						questions = questions + 1
				run = "not run"
				for contestanswer in contestanswers:
					if contestanswer[0] == contest:
						run = "run"
				if run == "not run":
					sendData = sendData + str(contest) + "\t" + str(questions) + " questions, " + run + "\n"
					continue
				maxques = []
				nicknames = []
				for contestanswer in contestanswers:
					if contestanswer[1] in nicknames:
						continue
					else:
						nicknames.append(contestanswer[1])
				for name in nicknames:
					maxquestionright = 0
					for contestanswer in contestanswers:
						if contestanswer[0] == contest:
							if contestanswer[1] == name:
								if contestanswer[3] == 'Correct':
									maxquestionright = maxquestionright + 1
					maxques.append(maxquestionright)
				topanswer = max(maxques)
				average = float(sum(maxques))/float(len(maxques))
				sendData = sendData + str(contest) + "\t" + str(questions) + " questions, " + run + ", average correct: " + str(average) + "; Maximum Correct: " + str(topanswer) + "\n"
			sendData = json.dumps(sendData)
			c.send(sendData.encode())
			continue

		elif tempfirst == 'r':
			try:
				temprest = temprest.strip(" ")
				contnumber = int(temprest)
			# print(contnumber)
			except:
				sendData = "Error: Please enter a valid number for r"
				sendData = json.dumps(sendData)
			else:
				sendData = ""
				if temprest in contests:
					questions = 0
					questionsarray = []
					for question in contestsques:
						if str(contnumber) == str(question[0]):
							questions = questions + 1
							questionsarray.append(question[1])
					run = "not run"
					for contestanswer in contestanswers:
						if contestanswer[0] == temprest:
							run = "run"
					if run == "not run":
						sendData = sendData + str(contnumber) + "\t" + str(questions) + " questions, " + run + "\n"
						sendData = json.dumps(sendData)
						c.send(sendData.encode())
						continue
					maxques = []
					nicknames = []
					questionscorrect = []
					for contestanswer in contestanswers:
						if contestanswer[1] in nicknames:
							continue
						else:
							nicknames.append(contestanswer[1])
					for name in nicknames:
						maxquestionright = 0
						for contestanswer in contestanswers:
							if contestanswer[0] == temprest:
								if contestanswer[1] == name:
									if contestanswer[3] == 'Correct':
										maxquestionright = maxquestionright + 1
						maxques.append(maxquestionright)
					topanswer = max(maxques)
					average = float(sum(maxques)) / float(len(maxques))
					sendData = sendData + str(contnumber) + "\t" + str(questions) + " questions, " + run + ", average correct: " + str(average) + "; Maximum Correct: " + str(topanswer) + "\n"

					for question in questionsarray:
						totalcorrect = 0
						totalquestions = 0
						for contestanswer in contestanswers:
							if contestanswer[0] == temprest:
								if question == contestanswer[2]:
									totalquestions = totalquestions + 1
									if contestanswer[3] == 'Correct':
										totalcorrect = totalcorrect + 1
						percentage = float(totalcorrect*100)/float(totalquestions)
						sendData = sendData + "\t" + str(question) + "\t" + str(percentage) + "% correct. \n"

					sendData = json.dumps(sendData)
					c.send(sendData.encode())
					continue

				else:
					sendData = "Error: Please enter a valid number for r or set Contest number using the s command."
					sendData = json.dumps(sendData)
					c.send(sendData.encode())
					continue


		else:
			#print("It's a dict")
			print("")
	else:
		sendData = json.dumps("")

	c.send(sendData.encode())
	save_obj(questionDict, "questions")


c.close()


