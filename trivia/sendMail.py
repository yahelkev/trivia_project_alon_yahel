import smtplib
import sys

if(len(sys.argv) != 3):
	print("to few or less ags! ")
	exit()

toaddrs  = sys.argv[1]
msg = sys.argv[2]
fromaddr = 'triviaAlonYahel@gmail.com'
username = fromaddr
password = 'triviaAlonYahel12345678'
server = smtplib.SMTP('smtp.gmail.com:587')
server.starttls()
server.login(username,password)
server.sendmail(fromaddr, toaddrs, msg)
server.quit()