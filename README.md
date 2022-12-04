# Computer_Networks-CS301


CS301: Computer Networks  
Assignment 1: Application Layer Protocols (HTTP &  DNS) 

Deadline: 31st August 2022, 11:59PM 
---------------------------------------------------------------------------------------------------- 
Goal of the Assignment: Study and understand application layer protocols (HTTP,  DNS) using packer analyzer and other tools. 
---------------------------------------------------------------------------------------------------- 
Part 1 
Instruction: Start packet capture just before opening https://www.iitbhilai.ac.in website and stop the packet capture once the complete page is loaded (or you can wait for 2 minutes and then stop the packet capture). Save the pcap file and answer  the following questions by analyzing the packet traces.  
1. When you browse IIT Bhilai main page (https://www.iitbhilai.ac.in) how  many get request is sent (how many of the GET request are for embedded  content and how many get request for the text)? Plot the IO graph for packets  sent to iitbhilai.ac.in and packets received from iitbhilai.ac.in [3 Points] 
2. For each HTTP GET requests as you see above, find out (i) the total amount  of data being received in the corresponding HTTP response message. [2 Points] 
3. For the response to your HTTP GET request, get the image reconstructed by  hex editor. [2 Points] 
4. Find the total amount of data being received when you access  https://www.iitbhilai.ac.in [2 Points] 
5. HTTP Conditional GET: Answer the following questions. [4 Points]
1 

a. Inspect the contents of the first HTTP GET request from your browser  to the server. Do you see an “IF-MODIFIED-SINCE” line in the HTTP  GET?  
b. Inspect the contents of the server response. Did the server explicitly  return the contents of the file? How can you tell?  
c. Now inspect the contents of the second HTTP GET request from your  browser to the server. Do you see an “IF-MODIFIED-SINCE:” line in  the HTTP GET? If so, what information follows the “IF-MODIFIED SINCE:” header?  
d. What is the HTTP status code and phrase returned from the server in  response to this second HTTP GET? Did the server explicitly return the  contents of the file? Explain. 
6. Find the throughput observed while browsing IIT Bhilai site under two cases  [3 Points] 
a. When no other traffic in the background 
b. When a large file download is going.  
the throughput calculation needs filtering only IIT Bhilai pages (from the get request  originated from your browser till the last response has arrived at end of the web  page). 
7. Along with IIT Bhilai website, access one more website of your choice and  answer the following questions. How many DNS packets have you observed  in total? [4 Points] 
a. Create a <Domain Name, IP> table by exploring the queries and the  answers in those DNS packets. The Domain Name will be the domain  for which you see a query, and the IP address will be the address that is  being returned against the corresponding query. 
b. Can you find out the IP of the DNS servers by exploring the DNS  packets?
2 

Part 2 
1. Surf a website (other than google.com) of your choice and discuss the end-to end process of web page loading using Wireshark. How much time it took to  load the page? Find out how many connections are used to download this  page, are these connections persistent or non-persistent? How many objects  have been transferred on these connections? Which object took the longest  time to download? [8 Points] 
2. The root server in the Internet are in domain root-servers.net. You can see the  list of all root servers using dig [DNS lookup utility] or using any  tool/command. [6 Points] 
Use dig to ask the root server the address of www.iitbhilai.ac.in, without  recursion. Go through the hierarchy from the root without recursion,  following the referrals manually, until you have found the address of  www.iitbhilai.ac.in 
List all the name servers involved to find out the IP address of the  www.iitbhilai.ac.in? 
Do the same exercise for 2 more websites with different top-level domains  (.com, .edu, .org, etc.)  
3. Find all the active TCP port on your system. Identify the ports and PIDs of  your web browser. Can you identify the port number and PID of specific TAB  in your browser? Find out if any of the services running in your system uses  the standard ports of HTTP, DHCP, DNS, SMTP, and FTP. [4 Points]
3 

Deliverables in a tar ball on GC: 
● Submission Guidelines: Upload the Assignment Report, pcap in GC as a tar  ball with file name as <your roll no>_<your name>.tar 
● Readable Report [2 Marks for report quality] enumerating steps followed  with screenshots for each of the important steps.  
○ Pcap trace collected and mention the command/tool used. 
○ Put the screenshots (mandatory) to validate your answers in the  report. 
Check Web sources for more information
4 


  <br>
  
  
  
  <br>
  
  
  
  </br>
  
  </br>

Assignment 2: Transport Layer and
Network Simulations using NS-3

-------------------------------------------------------------------------------------------------------------------------------
Goal of the Assignment:
Part 1: Study and understand Transport layer protocol using Wireshark and other tools.
Part 2: Understand NS-3 basics and use NS-3 to study variation in Congestion Window.
Part 3: Study and understand different TCP variants by reading research articles.
-------------------------------------------------------------------------------------------------------------------------------
Part 1: Wireshark/tshark/tcpdump [23 Points]
1. Download a large file (i. ubuntu image from Internet and ii. Ubuntu image from intranet).
60 seconds of observation is sufficient. Plot the following metrics from Wireshark and write
inferences.
a. Plot the estimated Round Trip Time (RTT) variation for intranet and internet
download [2 Points]
b. Plot the TCP Congestion window (or the difference in ack numbers - bytes
delivered) for both intranet and internet download. X-axis is time and Y-axis is
byted delivered (X ticks for each RTT , hence sum up the bytes delivered over
each RTT). [3 Points]
c. Get the flow graph (Statistics - flow graph) [2 Points].
d. What is the average throughput observed in both cases [2 Points].
e. Plot the the receiver congestion window advertised over time [2 Points].
f. Plot the number of 1-duplicate ack, 2-duplicate ack and 3-duplicate acks received
over time [1+1+1=3 Points].

2. Download a small file and identify the TCP 3-way handshake? [3 Points]
3. Ping a host and and capture the packets with wireshark. What kind of packets are generated
by the ping command? [3 Points]
4. Use nmap (using command nmap –PS [neighbours ip address]) to perform the host scan
(same as used in previous question) and capture the packets with wireshark. What kind of
packets are generated by nmap? [3 Points]
Intranet link: https://www.iitbhilai.ac.in/index.php?pid=itis_intranet

Part 2: NS-3 Basics and Understanding TCP Congestion
Window using NS-3 [15 Points]

1. Create a simple topology of two nodes (Node1, Node2) separated by a point-to-point link (you
can use first.cc). Setup a UdpClient on one Node1 and a UdpServer on Node2. Let it be of a fixed
data rate R1. Start the client application, and measure end to end throughput while varying the
latency of the link. Plot latency vs throughput graph. [5 Points]

2. Use TCP_Demo.cc file for this part. The file is similar to sixth.cc/seventh.cc files available in
examples/tutorial folder of NS-3. In this part of the assignment, you will use two different TCP
variants (NewReno and CUBIC) and study the changes in the congestion window over time. You
must run the simulations by varying TCP variants. The TCP variant can be changed as shown
below:
//TODO: TCP variant set to NewReno/TcpCubic
//Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue
(TcpNewReno::GetTypeId()));
Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue
(TcpCubic::GetTypeId()));

The following code will help you to get the congestion window trace file which is already
included in the TCP_Demo.cc
//trace cwnd
AsciiTraceHelper asciiTraceHelper;

Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("tcp-
demo.cwnd");

ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow",
MakeBoundCallback (&CwndChange, stream));

Use Gnuplot or any other tool to plot the CW values. You can use following Gnuplot script to
plot.
set terminal png
set output "CW.png"
set title "Congestion window Plot"
set xlabel "Time (Seconds)"
set ylabel "Congestion Window"
plot "tcp-demo.cwnd" using 1:2 with linespoints title "Old Cwnd", "tcp-demo.cwnd"
using 1:3 with linespoints title "New Cwnd"
These lines are copied in the CW.plt which you can run to plot congestion window over time.
After running you will get CW.png.
>> gnuplot CW.plt

Answer the following questions for both the TCP variants:
1. How many times did the TCP algorithm reduce the cwnd, and why? [4 Points]
2. Out of these two TCP variants which variant is better, and why? [3 Points]
3. What happens to congestion window in following cases: [3 Points]
i) Increase the link bandwidth
ii) Increase the link delay
iii) Increase the error-rate

Part 3: Study of TCP Congestion Control Algorithms [20 Points]

Explain and compare any four TCP congestion control algorithms (TCP Variants).

Out of four TCP congestion control algorithms, one must be TCP Cubic, at least one algorithm
should be from loss-based, delay based, and hybrid category. Definitions of these categories are
given below and some of the TCP variants are shown in the table.
1. Loss based TCP variants uses packet loss as an indication of Congestion.
2. Delay based TCP variants considers packet delay rather than packet loss as congestion in
the network.

3. In hybrid type of TCP variant both packet loss and delay of packet is considered as a
congestion in network.

Note: Refer research papers for different algorithms. Report should have following points
➔ Algorithm Details
➔ Suitable for which scenarios and where it fails
➔ Compare all four variants in the end and write inferences

Note for Part 3: Do not copy and paste from the paper, understand the algorithms. This
assignment will be evaluated through presentation by students. TAs can randomly pick some
students and ask them to present the submitted report for evaluation.

Deliverables in a tar ball on GC:

● Submission Guidelines: Upload the Assignment Report in GC as a tar ball with file name
as <your roll no>_<your name>.tar
● Readable Report [2 Marks for report quality] enumerating steps followed with
screenshots for each of the important steps.
○ Link to download the collected Pcap trace and mention the command/tool used.
○ Put the screenshots in the report for better clarity
● For all the experiments write the inferences which you have observed.
● If you want, you can use CAPTCP [TCP Analyzer] tool or any other tool for Part 1.

Check Web sources for more information
  
  
  
  
  <br>
  
  
  
  <br>
  
  
  
  </br>
  
  </br>
  
 
Assignment 3: TCP/UDP Client-Server
Implementation using Socket Programming

Deadline: 15th November 2022, 11:59PM (Considering Meraz-22)

Extended Deadline: 18th November 2022, 11:59PM

---------------------------------------------------------------------------------------------------------
Goal of the Assignment: Get familiar with socket programming and learn to
create your own client server applications using socket programming.
--------------------------------------------------------------------------------------------------------
Part 1: Client/Server Programming to transfer content of a
file [20 Points]
Create your own TCP/UDP client and server application to transfer content of a
file.
The client should request for the content of a file by providing the file name and
server should send the contents of that file to the client. For simplicity, assume
that the file is a simple text file that contains a set of words, with first word being
your name and the last word being “EOF” (end-of-file). The server should look
for the file in the local directory, if the file is not there it should send back a
message “404: File-not-Found”. After receiving this message, the client should
print an error message “File not found” and exit.
If the file is present, the server should read the first line of the file, which
contains your name, and send this message to the client. After receiving your
name, the client creates a local file (with a different file name from the
requested one) and sends a message Word_#1 to the server. This message
indicates that the client is requesting the first word. After receiving the message
Word_#1, the server sends the first word after your name to the client. The
client writes this word to the local file after receiving it and sends the message
Word_#2 to request the next word and so on. This process continues until the

client receives the keyword EOF. Once the client receives the keyword EOF, it
closes the local file after writing the last word to the file.
Part 2: Client/Server Programming for network analysis [30
Points]
1. (15 Points) Create your own UDP echo client and server application to
measure round trip time between client and server (similar to “ping”
command). The client should create a UDP socket and send echo packets
to server at a given interval, number of echo messages, and given packet
size (use command line arguments). For simplicity you can keep buffer
size same as packet size. On reception of the packet, server should send
the packet back to the client. The client on reception of the packet should
calculate and display the round-trip time. To calculate the round-trip time,
you can have the timestamp in the packet or/and use some unique
identifier in the packet. You should also calculate and print the loss
percentage at the end.
2. (15 Points) Create an iperf like application using the above developed
echo client and server program. Reduce the interval between two
consecutive UDP echo packets generated by client to increase the number
of echo packets sent from client for a given packet size. Calculate the
throughput and average delay observed every one second. Plot the
observed throughput and average delay vs time (1 second interval).
Part 3: Making Echo Client/Server “protocol Independent” [15
Points]
Revise echo client and server to be protocol independent (support both IPv4
and IPv6).
Hint 1: sockaddr is too small for sockaddr_in6. sockaddr_storage has enough

size to support both sockaddr_in and sockaddr_in6. (You will see this in server-
side program.)

Hint 2: integrate getaddrinfo to avoid typing IPv6 address on your CLI

Hint 3: you may use hostname (IPv4: “localhost”, IPv6:
“ip6-localhost” address to develop / demonstrate the software on ubuntu.
They’re written in “/etc/hosts”.
Part 4: Create your own client server application [15 Points]
Add any one feature to Client/Server and demonstrate them. In the report, you
must describe the new features with their benefit. Significance of the feature
will impact the marks given.
--------------------------------------------------------------------------------------------
Instructions for Implementation:
● You may choose any programing language (C, JAVA, Python, etc.)
● The software must be based on Socket Programming.
● Wrappers of API must not be used (messaging etc). Use send/recv or
read/write using TCP/UDP socket.
● Keep the record of Reference.
Deliverables in a tar ball on GC:
1. A report detailing your implementation detail and the results. The core
idea of your answer to each question. Better visibility like screenshot of
application will be appreciated. One single report file (<your roll
no>_<Name>_<Assignment3>.pdf for all three parts, include the code in
the report as well.
2. Screenshots of packet capture using Wireshark during your experiments.
3. Screenshots showing the working of your code.
4. All source codes and README as a separate file so that TAs and
instructor can compile source code and execute the binary anytime.
5. Submit all files in a single zip file named as ,<your roll
no>_<Name>_<Assignment3>.zip
Note: Plagiarism check will be done on your submitted code/report. This
assignment will be evaluated through presentation. In the presentation, you are
expected to explain and demonstrate the applications you built.
  
