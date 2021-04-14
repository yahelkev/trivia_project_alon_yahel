import socket

HOST = '127.0.0.1'
PORT = 6666     
MSG_SIZE = 5  

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as listening_socket:
        listening_socket.bind((HOST, PORT))
        listening_socket.listen()
        conn, addr = listening_socket.accept()
        with conn:
            print('Connected by', addr)
            while True:
                data = conn.recv(MSG_SIZE)
                if not data:
                    break
                conn.sendall(data)

    listening_socket.close()
if __name__ == "__main__":
    main()