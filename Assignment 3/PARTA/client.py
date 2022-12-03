import socket
host = 'local host'
port = 5000
s = socket.socket(socket.AF_INET,
				socket.SOCK_STREAM)
s.connect(('127.0.0.1', port))
input_file_name = input()
s.send(input_file_name.encode())
total_message_recieved = s.recv(1024)
each_word = total_message_recieved.decode()
if each_word == "404:file not found":
    print("404:file not found")
else:
    lenght_of_input_file = int(each_word)
    s.send(each_word.encode())
    name = s.recv(1024).decode()
    file = open("recieved_file.txt",'w') # the recieve output file 
    for i in range(1,lenght_of_input_file): 
        s.send(("word_#"+str(i)).encode()) # join the each word and encode into a file
        total_message_recieved = s.recv(1024) # total mesage
        if total_message_recieved.decode() != "EOF":    #  donnot cover the word EOF 
            print(total_message_recieved.decode()) 	# # printing the total nubmber of words except the EOF
            file.write(total_message_recieved.decode() + " ")  # write the each word in output file for every iteration
        else:
            break
s.close()  # close the function.
