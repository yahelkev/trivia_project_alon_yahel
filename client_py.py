import socket
import json
import sys

MSG_SIZE = 5  
MAX_PORT = 65535
MIN_PORT = 1024
HOST_IP = '127.0.0.1'

# has request headers which will get input
REQUESTS = {
	"login": ["username", "password"],
	"signup": ["username", "password", "email"]
}

def openSocket():
	try:
		res = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		print ("Socket successfully created")
		return res

	except socket.error as err:
		print ("socket creation failed with error %s" %(err))
		sys.exit()

def getPortFromUser():
	result = 0
	while (result < MIN_PORT  or result > MAX_PORT):
		try:
			result = int(input("enter port number (%d - %d):"%(MIN_PORT, MAX_PORT)))
		except ValueError:
			print("Oops!  That was no valid number.  Try again...")
	return result

def connectToServer(server_socket, port):
	print("connecting...")
	try:
		server_socket.connect((HOST_IP, port))
	except:
		print("can not connect to server on port: %d" %port)
		sys.exit()

def getMassage():
	request = input("choose a request (" + ", ".join(REQUESTS.keys()) + "): ")
	if request not in REQUESTS.keys():
		print("invalid request!")
		raise ValueError
	# get input for request keys
	requestInput = []
	for key in REQUESTS[request]:
		requestInput += [input("Enter %s: " % key)]
	print("%s - %s" % (", ".join(REQUESTS[request]), ", ".join(requestInput)))
	# return json

def main():
	server_socket = openSocket()
	port = getPortFromUser()
	connectToServer(server_socket, port)

	try:
		msg = getMassage()
	except ValueError:
		return
	server_socket.sendall(msg.encode())
	print("msg from server:" + server_socket.recv(MSG_SIZE).decode())

if __name__ == "__main__":
	main()
