import socket
host = 'local host'
port = 5000
s = socket.socket(socket.AF_INET,
			socket.SOCK_STREAM)
s.bind(('', port))
s.listen(1)
c, addr = s.accept()
print("CONNECTION FROM:", str(addr))
msg = c.recv(1024)
file_name = msg.decode()
print(file_name)
try:
    file = open(file_name)
    lst = file.read().split() # print(lst)
    c.send(str(len(lst)).encode())
    msg = c.recv(1024)
    c.send(lst[0].encode())
    while True:
        msg = c.recv(1024)
        print(msg.decode())
        c.send(lst[int(msg.decode()[6:])].encode())
        if int(msg.decode()[6:]) == len(lst)-1:
            break
except FileNotFoundError:
    c.send("404:file not found".encode())
c.close()

