# Import the necessary libraries

# Import the necessary functions from the socket library
from socket import AF_INET, SO_REUSEADDR, SOCK_STREAM, SOL_SOCKET, error, socket
from sys import exit
from time import sleep
def bind_socket():
    try:
        print("Binding the Port:", port)

        s.bind((host, port))
        s.listen(5)
    # Socket is unable to bind to the specific socket address
    except error as msg:
        # We seek to try binding again
        print("Socket Binding error", msg)
        # Seek to retry
        print("Retrying...")
        # Wait for an interval of 5 seconds for the function to end
        sleep(5)
        # Retry binding by calling the same function recursively
        bind_socket()
host = ""
# Standard port is initialised
port = 8888
s = None
try:
    s = socket(AF_INET, SOCK_STREAM)
except error as msg:
    print("Socket creation error:", msg)
    s.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
bind_socket()
print("Server Listening...")
conn, address = s.accept()
print("Connection has been established! |", "IP", address[0], "| Port",
      address[1])
f = open("Input_file.txt", "rb")
l = f.read(1024)
while l:
    conn.send(l)
    l = f.read(1024)
f.close() # wating for file for 5 second to close fully and get saved 
sleep(5)
# This is important as it will tell the client to close the connection
conn.send(str.encode("Thank you for connecting"))  
print("Part4 Completed")  # displaying messge
conn.close()  # clossing of connection that has been established between server and client
s.close()  # clonsing of socket
exit()  # eciting of programm
