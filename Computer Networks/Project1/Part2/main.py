from socket import *
import timeit
# Ayham Maree - 1191408
# Sara Ammar - 1191052
# Alaa Sehawil - 1191741
# Mahmoud Shouman -1191648
###############################################
serverName = input("Enter URL which will be used in http request?\n") #get the url of the website from user
name = "Hostname:" + str(serverName)  + "\r\n\r\n" #convert the url to string hostname
serverPort=80 # integer that is the port number we use
clientSocket = socket(AF_INET,SOCK_STREAM)  #make a socket and give it the name clientsocket
start = timeit.default_timer() # function to start time from the time we send the request to conect to the server
clientSocket.connect((serverName,serverPort)) #connect between the clientsocket and serversocket
sentence =input("Input lowercase sentence:") #input from user the message we will send to the website
clientSocket.send("HEAD / HTTP/1.1 \r\n".encode()) #send the head of http response after encode it
clientSocket.send(sentence.encode()) #send the message we need as a string
clientSocket.send(name.encode()) #send the server name (hostname) to the socket after encode it
modifiededSentence = clientSocket.recv(1024) #modifiy the sentence by the famous client proxy 1024
print("From Server:",modifiededSentence.decode()) # decode the message and get the http response after networking between client and server using socket
print ('time taken ', timeit.default_timer()-start,' milli seconds') #print the  spent time of the opreation
clientSocket.close() #close the client
##################################################