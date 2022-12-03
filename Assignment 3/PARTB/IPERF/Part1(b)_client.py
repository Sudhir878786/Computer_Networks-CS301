# Import the necessary libraries

from datetime import datetime as dt
from decimal import Decimal
from random import choice, randint

# Import the necessary functions from the socket library
from socket import AF_INET, SOCK_DGRAM, socket, timeout
from string import ascii_lowercase, digits
from time import sleep

from matplotlib.pyplot import grid, plot, show, subplot, title, xlabel, ylabel
IP_ADDRESS_OF_SERVER = ("127.0.0.1", 20001)

# Get the necessary inputs from the user

# User-defined - Number of echo messages
TOTAL_NUMBER_OF_MESSAGES = int(input("Enter the number of echo messages to be sent: "))

# User-defined - Given interval
interval = Decimal(input("Enter the interval: "))
SIZE_OF_BUFFER = int(input("Enter the packet size in bytes: "))
CLIENT_SOCKET_OF_UDP = socket(family=AF_INET, type=SOCK_DGRAM)
CLIENT_SOCKET_OF_UDP.settimeout(1)
avg_round_trip_time = 0

# Count of packets that have completed a round trip and not been dropped
NUMBER_OF_SUCCESS_PACKET = 0

# Calibration of Buffer Size
calibration_msg = ("C :" + str(SIZE_OF_BUFFER)).encode()

# Print appropriate message to the user
print("Calibrating the size of the buffer...")

# Appropriate message is sent to the server
CLIENT_SOCKET_OF_UDP.sendto(calibration_msg, IP_ADDRESS_OF_SERVER)
ACKNOWLEDGMENT_MESSAGE = CLIENT_SOCKET_OF_UDP.recvfrom(SIZE_OF_BUFFER)
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
VERSION = ' v1.5.1: https://github.com/johnathan79717/codeforces-parser'
RED_F = '\033[31m'
GREEN_F = '\033[32m'
BOLD = '\033[1m'
NORM = '\033[0m'
TIME_CMD = '`which time` -o time.out -f "(%es)"'
TIME_AP = '`cat time.out`'

# Problems parser.
class CODE(HTMLParser):

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
class CODE(HTMLParser):

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

# Parses each problem page.
def parse_problem(folder, contest, problem):
    url = 'http://codeforces.com/contest/%s/problem/%s' % (contest, problem)
    html = urlopen(url).read()
    parser = CodeforcesProblemParser(folder)
    parser.feed(html.decode('utf-8'))
    # .encode('utf-8') Should fix special chars problems?
    return parser.num_tests

# Parses the contest page.
def parse_contest(contest):
    url = 'http://codeforces.com/contest/%s' % (contest)
    html = urlopen(url).read()
    parser = CodeforcesContestParser(contest)
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
def new_interval(interval_value):
    to_be_lost = Decimal(interval_value / 10)
    new_interval_value = Decimal(interval_value - to_be_lost)
    return new_interval_value
AVG_THROUGHPUT = []
AVG_DELAY = []
TIME = []
counter_of_time_second = 0
initial_counter = 0
while TOTAL_NUMBER_OF_MESSAGES > 0:
    if TOTAL_NUMBER_OF_MESSAGES == 0:
        break
    counter_of_time_second += 1
    print("Second", counter_of_time_second, ":")
    average_delay_in_time = 0
    NUMBER_OF_SUCCESS_PACKET = 0
    second_start = Decimal(dt.now().timestamp())
    while Decimal(dt.now().timestamp()) - second_start <= 1:
        if initial_counter > 1:
            tosleep = 1
            initial_counter -= 1
            print("No packet will be sent in this second")
        else:
            tosleep = Decimal(initial_counter)
        sleep(float(tosleep))
        if TOTAL_NUMBER_OF_MESSAGES == 0:
            break
        if tosleep == 1:
            break
        client_msg = "".join([
            choice(ascii_lowercase + digits) for _ in range(randint(10, 20))
        ]).encode()
        CLIENT_SOCKET_OF_UDP.sendto(client_msg, IP_ADDRESS_OF_SERVER)
        send_time_stamp = Decimal(dt.now().timestamp())
        TOTAL_NUMBER_OF_MESSAGES -= 1
        try:
            server_msg = CLIENT_SOCKET_OF_UDP.recvfrom(SIZE_OF_BUFFER)
            print("Received message:", server_msg[0].decode())
        except timeout:
            print("Packet lost...\nSending next packet")
            continue
        receive_time_stamp = Decimal(dt.now().timestamp())
        total_round_trip_time_to_sending_time = Decimal(receive_time_stamp - send_time_stamp)
        NUMBER_OF_SUCCESS_PACKET += 1
        average_delay_in_time += Decimal(total_round_trip_time_to_sending_time)
        sleeptime = Decimal(max(interval - total_round_trip_time_to_sending_time, 0))
        interval = new_interval(interval)
        time_taken_to_end_process = Decimal(Decimal(dt.now().timestamp()) - second_start)
        if time_taken_to_end_process > 1:
            initial_counter = Decimal(tosleep)
            break
        if time_taken_to_end_process + sleeptime > 1 and time_taken_to_end_process <= 1:
            tosleep = Decimal(1 - time_taken_to_end_process)
            initial_counter = Decimal(sleeptime - (1 - time_taken_to_end_process))
        else:
            tosleep = Decimal(sleeptime)
        sleep(float(tosleep))
    if NUMBER_OF_SUCCESS_PACKET == 0:
        average_delay_in_time = 0
    else:
        average_delay_in_time /= NUMBER_OF_SUCCESS_PACKET
    avg_throughput = NUMBER_OF_SUCCESS_PACKET * SIZE_OF_BUFFER * 2 # The Average Throughput is given by the total data transferred by the total time passed (in bytes per second)     # The total data #transferred is the successful packets transferred multiplied by the data carried by each packet     # Since we need to calculate Average Throughput every one second, we don't need to explicitly #divide by 1     # As the size of the buffer is taken in bytes, no change in unit is required     # Since the same packet is transferred from client and vice-versa, the data transferred is twice (hence #the factor of TWO)
    print("The Average Throughput is", avg_throughput, "bytes/seconds")
    print("The Average Delay is", average_delay_in_time, "seconds")
    AVG_THROUGHPUT.append(avg_throughput)
    AVG_DELAY.append(average_delay_in_time)#  apending the average delay  with timer 
    TIME.append(counter_of_time_second) #  appending the plotting time interval scale
subplot(1, 2, 1)  #   initial cordinaties need for plotting graph
plot(TIME, AVG_THROUGHPUT, "red")  # ploting the graph between time interval and average throughput with red coulour
xlabel("Time (in seconds)")# # avegare time interval label on x axis
ylabel("Average Throughput (bytes/seconds)")   # anotther graph y label as Average thoughput
title("Average Throughput (bytes/seconds) vs Time (in seconds)")   # addding title as label for Y and X axis
grid(True)  # make grid true for bettter results
subplot(1, 2, 2)  # initial condinates for graph
# Plot the Average Delay values in seconds on the
plot(TIME, AVG_DELAY, "blue") # These will plot the graph betwwen the averge thoughput and time interval using blue line
xlabel("Time (in seconds)") # x axis label in second printed onn graph
# Label the Y-axis in the subplot with the Average Delay in seconds
ylabel("Average Delay (in seconds)") # Y-axis label  that is average delay in second
title("Average Delay (in seconds) vs Time (in seconds)") # adding the title for graph as Average delay vs time interval
grid(True)  #  Make a grid out of the graph to give better approximation result will be better
show()  # callng the show funcion  to display the graph
# Print appropriate message to the user
print("Graph Plotting is Complete") #  heading add to top of the graph that we need to plot ie the graph betweeen the time interval and throughput
# Print appropriate message to the user
print("Finished sending, Terminating the connection...") #printing  the last message and the ending termination message form server
# Send the message to exit to the server, to terminate the process
CLIENT_SOCKET_OF_UDP.sendto("exit".encode(), IP_ADDRESS_OF_SERVER)# # Receive the termination message from the server 
exit_msg = CLIENT_SOCKET_OF_UDP.recvfrom(SIZE_OF_BUFFER) # the reply message that we need to print from the server# Indicates that the server has closed the connection
# Print appropriate message to the user
print(exit_msg[0].decode("utf-8")) # printing the expected output we needed
