import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
host = "localhost"
port = 9999
message = "Hello"

r = sock.sendto(message.encode(), (host, port))
print("Sent data." + str(r))
sock.close()