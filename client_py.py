import socket
import json
import sys

MSG_SIZE = 5  
MAX_PORT = 65535
MIN_PORT = 1024
HOST_IP = '127.0.0.1'
REQUEST_CODE_BYTES = 1
CONTENT_LENGTH_BYTES = 4

# has request headers which will get input
REQUESTS = {
	"login": {
		"keys": ["username", "password"],
		"code": 1
	},
	"signup": {
		"keys": ["username", "password", "email"],
		"code": 2
	},
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
	""" function gets input for a request from user
	returns request code and content"""
	request = input("choose a request (" + ", ".join(REQUESTS.keys()) + "): ")
	if request not in REQUESTS.keys():
		print("invalid request!")
		raise ValueError
	# get input for request keys
	request_input = []
	for key in REQUESTS[request]["keys"]:
		request_input += [input("Enter %s: " % key)]
	# return message info
	return (REQUESTS[request]["code"], fillJson(REQUESTS[request], request_input))

def fillJson(keys, values):
	"""function fills a json object with the specified keys and values"""
	dictionary = dict(zip(keys, values))
	return json.dumps(dictionary)

def sendRequest(server_socket, code, content):
	"""function sends a request to the server in the trivia protocol"""
	encoded_request = code.to_bytes(REQUEST_CODE_BYTES, "big")
	encoded_request += len(content).to_bytes(CONTENT_LENGTH_BYTES, "big")
	encoded_request += content.encode()
	server_socket.sendall(encoded_request)

def main():
	server_socket = openSocket()
	port = getPortFromUser()
	connectToServer(server_socket, port)

	try:
		req = getMassage()
	except ValueError:
		return
	sendRequest(server_socket, *req)
	print("msg from server:" + server_socket.recv(MSG_SIZE).decode())

if __name__ == "__main__":
	main()
