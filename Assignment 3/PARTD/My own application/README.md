Working:
1. First create compiled object file for server as :
	cc tcpserver.c -o server
2. Create compiled object file for client as :
	cc tcpclient.c -o client
3. Run server as ./server
4. Run client as ./client
5. Default admin account details -> admin:admin

NOTE : 
1. You need to change ip:port according to your machine if running on different machines.
2. You can see the image(flow.jpg) attached for code flow.

Bugs/Faliure:
1. Balance update will not happen if you've -ve balance & deposit amount will not able to recover it fully.
2. Once user is deleted can be added back as new user again but it will not show details of it.

Techniques Used:
1. Socket Programming in C
2. File Handling in C(open, read, write, lseek system calls)
3. File Locking in C (fcntl)