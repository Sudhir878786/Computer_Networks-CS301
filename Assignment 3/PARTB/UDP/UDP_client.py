# Import the necessary libraries
#!/usr/bin/env python
from __future__ import print_function

# Python 2->3 libraries that were renamed.
try:
    from urllib2 import urlopen
except ImportError:
    from urllib.request import urlopen
try:
    from HTMLParser import HTMLParser
except ImportError:
    from html.parser import HTMLParser
# Other libraries.
from sys import argv
from subprocess import call
import re
import os
# User modifiable constants:
TEMPLATE = 'main.py'
SAMPLE_INPUT = 'input'
SAMPLE_OUTPUT = 'output'
MY_OUTPUT = 'my_output'
# Do not modify these!
VERSION = ' Parser v1.5.1: https://github.com/johnathan79717/-parser'
RED_F = '\033[31m'
GREEN_F = '\033[32m'
BOLD = '\033[1m'
NORM = '\033[0m'
TIME_CMD = '`which time` -o time.out -f "(%es)"'
TIME_AP = '`cat time.out`'
# Problems parser.
class func3(HTMLParser):

    def __init__(self, folder):
        HTMLParser.__init__(self)
        self.folder = folder
        self.num_tests = 0
        self.testcase = None
        self.start_copy = False
        self.end_line = None

    def handle_starttag(self, tag, attrs):
        if tag == 'div':
            if attrs == [('class', 'input')]:
                self.num_tests += 1
                self.testcase = open(
                    '%s/%s%d' % (self.folder, SAMPLE_INPUT, self.num_tests), 'wb')
            elif attrs == [('class', 'output')]:
                self.testcase = open(
                    '%s/%s%d' % (self.folder, SAMPLE_OUTPUT, self.num_tests), 'wb')
        elif tag == 'pre':
            if self.testcase != None:
                self.start_copy = True

    def handle_endtag(self, tag):
        if tag == 'br':
            if self.start_copy:
                self.testcase.write('\n'.encode('utf-8'))
                self.end_line = True
        if tag == 'pre':
            if self.start_copy:
                if not self.end_line:
                    self.testcase.write('\n'.encode('utf-8'))
                self.testcase.close()
                self.testcase = None
                self.start_copy = False

    def handle_entityref(self, name):
        if self.start_copy:
            self.testcase.write(self.unescape(('&%s;' % name)).encode('utf-8'))

    def handle_data(self, data):
        if self.start_copy:
            self.testcase.write(data.encode('utf-8'))
            self.end_line = False
# Contest parser.
class pas_clinet(HTMLParser):

    def __init__(self, contest):
        HTMLParser.__init__(self)
        self.contest = contest
        self.start_contest = False
        self.start_problem = False
        self.name = ''
        self.problem_name = ''
        self.problems = []
        self.problem_names = []

    def handle_starttag(self, tag, attrs):
        if self.name == '' and attrs == [('style', 'color: black'), ('href', '/contest/%s' % (self.contest))]:
            self.start_contest = True
        elif tag == 'option':
            if len(attrs) == 1:
                regexp = re.compile(r"'[A-Z]'") # The attrs will be something like: ('value', 'X')
                string = str(attrs[0])
                search = regexp.search(string)
                if search is not None:
                    self.problems.append(search.group(0).split("'")[-2])
                    self.start_problem = True

    def handle_endtag(self, tag):
        if tag == 'a' and self.start_contest:
            self.start_contest = False
        elif self.start_problem:
            self.problem_names.append(self.problem_name)
            self.problem_name = ''
            self.start_problem = False

    def handle_data(self, data):
        if self.start_contest:
            self.name = data
        elif self.start_problem:
            self.problem_name += data
#Parses each problem page.
def parse_problem(folder, contest, problem):
    url = 'http://.com/contest/%s/problem/%s' % (contest, problem)
    html = urlopen(url).read()
    parser = func3(folder)
    parser.feed(html.decode('utf-8'))
    # .encode('utf-8') Should fix special chars problems?
    return parser.num_tests
# Parses the contest page.
def parse_contest(contest):
    url = 'http://.com/contest/%s' % (contest)
    html = urlopen(url).read()
    parser = ContestParser(contest)
    parser.feed(html.decode('utf-8'))
    return parser
# Generates the test script.
def generate_test_script(folder, num_tests, problem):
    with open(folder + 'test.sh', 'w') as test:
        test.write(
            '#!/bin/bash\n'
            'DBG=""\n'
            'while getopts ":d" opt; do\n'
            '  case $opt in\n'
            '    d)\n'
            '      echo "-d was selected; using DEBUG mode!" >&2\n'
            '      DBG="-d"\n'
            '      ;;\n'
            '    \?)\n'
            '      echo "Invalid option: -$OPTARG" >&2\n'
            '      ;;\n'
            '  esac\n'
            'done\n'
            '\n'
            'INPUT_NAME='+SAMPLE_INPUT+'\n'
            'OUTPUT_NAME='+SAMPLE_OUTPUT+'\n'
            'MY_NAME='+MY_OUTPUT+'\n'
            'rm -R $MY_NAME* &>/dev/null\n')
        test.write(
            'for test_file in $INPUT_NAME*\n'
            'do\n'
            '    i=$((${{#INPUT_NAME}}))\n'
            '    test_case=${{test_file:$i}}\n'
            '    chmod 711 {7}.py\n'
            '    if ! {5} ./{7}.py $DBG < $INPUT_NAME$test_case > $MY_NAME$test_case; then\n'
            '        echo {1}{4}Sample test \#$test_case: Runtime Error{2} {6}\n'
            '        echo ========================================\n'
            '        echo Sample Input \#$test_case\n'
            '        cat $INPUT_NAME$test_case\n'
            '    else\n'
            '        if diff --brief --ignore-trailing-space $MY_NAME$test_case $OUTPUT_NAME$test_case; then\n'
            '            echo {1}{3}Sample test \#$test_case: Accepted{2} {6}\n'
            '        else\n'
            '            echo {1}{4}Sample test \#$test_case: Wrong Answer{2} {6}\n'
            '            echo ========================================\n'
            '            echo Sample Input \#$test_case\n'
            '            cat $INPUT_NAME$test_case\n'
            '            echo ========================================\n'
            '            echo Sample Output \#$test_case\n'
            '            cat $OUTPUT_NAME$test_case\n'
            '            echo ========================================\n'
            '            echo My Output \#$test_case\n'
            '            cat $MY_NAME$test_case\n'
            '            echo ========================================\n'
            '        fi\n'
            '    fi\n'
            'done\n'
            .format(num_tests, BOLD, NORM, GREEN_F, RED_F, TIME_CMD, TIME_AP, problem))
    call(['chmod', '+x', folder + 'test.sh'])

from datetime import datetime as dt
from decimal import Decimal
from random import choice
from random import randint
from string import ascii_lowercase
from string import digits
from time import sleep
# Import the necessary functions from the socket library
from socket import AF_INET
from socket import socket
from socket import SOCK_DGRAM
from socket import timeout
# Set the server IP address and port tuple to a localhost and arbitrary value respectively
# Readjust this value if you want to communicate with another client on the same network
ip_address_of_server = ("127.0.0.1", 20001)
# Get the necessary inputs from the user
# User-defined - Number of echo messages
totol_number_of_messages = int(input("Enter the number of echo messages to be sent: "))
# User-defined - Given time_interval
time_interval = Decimal(input("Enter the time_interval: "))
# User-defined - Given Packet Size
# As the size of the buffer is same as the packet size
size_of_buffer = int(input("Enter the packet size in bytes: "))
# Create a UDP socket at client side
client_socket_var_for_UDP = socket(family = AF_INET, type = SOCK_DGRAM)  # 
client_socket_var_for_UDP.settimeout(1) # set the UDP timeout to be 1 second
average_round_trip_time = 0  # variable to average round trip time 
total_number_of_packets_passed = 0 # variable to store the tootal number of packets passedd
message_calib_of_buffer_size = ("C :" + str(size_of_buffer)).encode() # encoding the message recieved from calibration buffer size
print("Calibrating the size of the buffer...") # printing the current size of buffer
client_socket_var_for_UDP.sendto(message_calib_of_buffer_size, ip_address_of_server) # current buffer size is send to current ip address
ackwnoledgement_message = client_socket_var_for_UDP.recvfrom(size_of_buffer) # ackwonolodgenment messagee from client to server that mesage has recieved
for iteration_in_message in range(1, totol_number_of_messages + 1):  #initiazing the loop from 1 to total number of messages + 1 
    print("Sending message for packet number", iteration_in_message, "of size", size_of_buffer, "bytes") #printing the sending message from current packtes in each oteration.
    total_time_in_each_time_stamp = Decimal(dt.now().timestamp()) # decimal value fo total time in each time stamp till currenrt iteration in message.
    message_recieved_from_client = ''.join([choice(ascii_lowercase + digits) for _ in range(randint(10, 20))]).encode() # encode the message recieve from the  client and join them together
    client_socket_var_for_UDP.sendto(message_recieved_from_client, ip_address_of_server) # messasge recieve from the client is send to ip address of server
    try:   # try and except condtion 
        message_recieved_from_server = client_socket_var_for_UDP.recvfrom(size_of_buffer)  #reform the client socket for UDP assing to message recieve from the server.
    except timeout:  # if message from the serverif out of the range of the size of the buffer
        print("Packet lost...\nSending next packet") # printing if current time there is a packets loss occours
        continue    # continue if timeouts occours
    # Estimate the current timestamp to put a timestamp of receiving the packet
    # It marks the receiving time of the echo message
    incoming_time_stamp_from_packets = Decimal(dt.now().timestamp()) # taking the Decimal value of incoming timestamp from packets
    toal_round_trip_after_recieved = incoming_time_stamp_from_packets - total_time_in_each_time_stamp # subtracting the incoming timestamp from packets to total time in taken in each time stamp
    print("Message from Server is", message_recieved_from_server[0].decode("utf-8"))#  priting the messsage recieved from the server
    print("Round Trip Time for packet", iteration_in_message, "is", toal_round_trip_after_recieved, "seconds") # printing the round trip time of each packets
    average_round_trip_time += toal_round_trip_after_recieved # adding total roundtrip timeafter recieve.
    total_number_of_packets_passed += 1 # increment in total number of packets passed
    sleep(float(max(time_interval - toal_round_trip_after_recieved, 0))) # sleep when time interval - total round trip timeafter recieve is maximum
number_of_packets_get_loss_while_transfered = (totol_number_of_messages - total_number_of_packets_passed)# subtracting the number of messages with total nuber of packets pass
number_of_packets_get_loss_while_transfered_percentage = Decimal(number_of_packets_get_loss_while_transfered / totol_number_of_messages) * 100 # Taking the decinmal value after dividing numberof packets with totalnnumbes of messages
average_round_trip_time /= total_number_of_packets_passed # divide the average round trip time with total number of packets pass in each iteartion
print("Average Round Trip Time is", average_round_trip_time, "seconds")#    printing the average round trip time
print("The number of packets dropped is", totol_number_of_messages - total_number_of_packets_passed) #  printing the number of packets drop
print("The packet loss percentage is", number_of_packets_get_loss_while_transfered_percentage, "%")# if packect loss it will print packet loss in percentage
print("Finished sending, Terminating the connection...") # printing the finishing send and termination line
client_socket_var_for_UDP.sendto('exit'.encode(), ip_address_of_server)# exiting the ip address
leaving_message = client_socket_var_for_UDP.recvfrom(size_of_buffer) #taking the lesving message 
print(leaving_message[0].decode("utf-8")) # printing the leavib=ng message
