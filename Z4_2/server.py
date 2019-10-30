import socket

sock=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
host="localhost"
port=9999
sock.bind((host,port))

msg, addr = sock.recvfrom(1024)
print("Message from ", addr, ": ", msg.decode())

sock.close()