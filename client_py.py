
import socket
import sys

try:
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	print ("Socket successfully created")
except socket.error as err:
	print ("socket creation failed with error %s" %(err))

port = 0
while (port < 1024  or port > 65535):
	port = int(input("enter port number (1024 - 65535):"))

print("connecting...")

try:
	host_ip = '127.0.0.1'
except socket.gaierror:
	print ("there was an error resolving the host")
	sys.exit()

try:
	s.connect((host_ip, port))
except:
	print("can not connect to server on port: %d" %port)
	sys.exit()

msg = ""
while(msg != "hello"):
	msg = input("enter message(hello to send): ")[:5]
	print("you enterd: %s"%msg)

s.sendall(msg.encode())
print("msg from server:" + s.recv(1024).decode())