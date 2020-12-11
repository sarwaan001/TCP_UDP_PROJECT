# TCP_UDP_PROJECT
Manual Pages for each file:
Name: 
./cserver – Server used for ./contestmeister and ./contestant for a quiz contest application.
Synopsis: 
./cserver
Description: 
Server used for ./contestmeister and ./contests for a quiz contest application. This server contains necessary functions to connect to ./contestmeister, connect to ./contestants, and fulfil commands called by ./contestmeister, which includes set contest ‘s’, create questions ‘p’, delete questions ‘d’, get questions ‘g’, check question ‘c’, kill server ‘k’, end client ‘q’, help ‘h’, start contest ‘b’, add questions to contest ‘a’, list contests ‘l’, and review contest ‘r’.
Options:
There are no options for ./cserver

Name:
./contestmeister
Synopsis:
./cserver [host name] [host port]
Description:
Interface to connect to ./cserver. This function requires 2 arguments, the host name and the host port (which are both given by the host running ./cserver).
	p [question number]\n[tag]\n[question]\n.\n[choice 1]\n.\n[choice n]\n.\n.\n[answer]
This function creates a question with a specified question number, tag, question, answer choices, and answer.
	d [question number]
This function deletes a specified question.
	g [question number]
This function obtains a question number.
	c [question number] [answer] 
This function checks if an answer is correct for a given question.
	k
This function kills the server.
	q
This function ends the client.
	s [contest number]
This function initializes a contest number.
	a [contest number] [question number]
This function adds a question to a given initialized contest.
	b [contest number]
This function starts a function with an initialized contest number. This function returns a port for ./contestant.
	l
This function lists all initialized contests and supplies average correct answers and maximum score.
	r [contest number]
This function shows the average correct answers for the contest, maximum score for the contest, and percentage correct per question for a given contest number.
	h
This function supplies all the functions to the contestmeister.
Options:
./contestmeister [host name] [host port]
Connects ./contestmeister to a given ./cserver using the ./cserver host name and host port.

./contestmeister [host name] [host port] [cmd text file]
The cmd text file option allows a textfile to run commands in contestmeister.

Name:
./contestant
Synopsis:
./contestant [host name] [host port]
Description:
Interface to connect to ./cserver question interface. This function takes 2 arguments: The host name (cserver host name) and the host port (supplies by the b command).
Options:
./contestant [host name] [host port]
Connects ./contestant to a ./cserver with the supplied host name and host port.
