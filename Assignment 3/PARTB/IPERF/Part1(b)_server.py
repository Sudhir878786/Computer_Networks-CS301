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
VERSION = ' Parser v1.5.1: https://github.com/johnathan79717/codeforces-parser'
RED_F = '\033[31m'
GREEN_F = '\033[32m'
BOLD = '\033[1m'
NORM = '\033[0m'
TIME_CMD = '`which time` -o time.out -f "(%es)"'
TIME_AP = '`cat time.out`'

# Problems parser.
class socketClientss(HTMLParser):

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
class func2(HTMLParser):

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
    parser = func2(contest)
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

from random import choice
from random import randint
from string import ascii_lowercase
from string import digits
from socket import AF_INET
from socket import socket
from socket import SOCK_DGRAM
server_IP_address_port = ("127.0.0.1", 20001)
maximum_buffer_size = 1024
UDP_server_socket = socket(family = AF_INET, type = SOCK_DGRAM)
UDP_server_socket.bind(server_IP_address_port)
print("UDP server has binded and is waiting for client connection")
while True:
    input_message, input_ip_address = UDP_server_socket.recvfrom(maximum_buffer_size)
    message_recieved_from_client = input_message.decode()
    print("Message from the Client is", message_recieved_from_client)
    if message_recieved_from_client == 'exit':
        UDP_server_socket.sendto("Terminated the connection successfully.".encode(), input_ip_address)
        print("Exiting...")
        break
    if message_recieved_from_client[0] == 'C':
        print("The IP Address and port of the Client is", input_ip_address)
        maximum_buffer_size = int(message_recieved_from_client.split(':')[1])
        print("Updated buffer size to", maximum_buffer_size, "bytes")
        UDP_server_socket.sendto(("Buffer Size Calibrated at Server.").encode(), input_ip_address)
        continue
    generating_random_number = randint(1, 100)
    if generating_random_number == 50:
        continue
    output_reply_from_server = ''.join([choice(ascii_lowercase + digits) for _ in range(generating_random_number)]).encode()
    UDP_server_socket.sendto(output_reply_from_server, input_ip_address)
