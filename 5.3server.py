import socket
import sys
import json

mydata = {"id": 717445, "name": "Iqbal", "age": "23"}
sendData = json.dumps(mydata)

s = socket.socket()
print("Socket successfully created")

port = 8080

s.bind(('', port))
print("socket binded to " + str(port))

s.listen(5)
print("socket is listening")

while True:
        c, addr = s.accept()
        print("Got connection from" + str(addr))

        c.sendall(bytes(sendData,encoding="utf-8"))
        buffer = c.recv(1024)
        print(buffer)
c.close()
