# Import the necessary libraries

# Import the necessary functions from the socket library
from socket import AF_INET
from socket import error
from socket import socket
from socket import SOCK_STREAM
# Uninitialised socket at client
s = None
# Standard host is initialised
host = 'localhost'
# Standard port is initialised
port = 8888
try:
    s = socket(AF_INET, SOCK_STREAM)
except error as msg:
    print("Socket creation error:", msg)
try:
    s.connect((host, port))
except error as msg:
    print("Socket connection error:", msg)  # printing connerction error if there do so
with open('output_file.txt', 'w') as f:  # witting the and making of output file 
    print('opening file')  # printing and showing that file is openingn
    while True:  # while true start recieving the messsage fromt the server
        print('receiving data from the Input_file...')
        data = s.recv(1024).decode("utf-8") # recieving the current server message as string inpit
        if data == 'Thank you for connecting': # condititon that if you wanrt to connect   data recieval in complete
            print('Successfully generated output_file') # ackwonolodgment menssage that output file has been generated
            print('Thank you for connecting') # printing the thankyou message
            break #break if as ther is no more data to fetch from thefile 
        f.write(data) # write the data that in comming form the server
f.close()  # close thecurrent file that we have written
s.close() # close the socket that we  have open in startking
