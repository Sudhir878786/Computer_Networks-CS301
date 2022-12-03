#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor-module.h"
#include <stdio.h>
#include "ns3/gnuplot.h"
#include <string.h>
// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
 
using namespace ns3;
 
NS_LOG_COMPONENT_DEFINE("FirstScriptExample");
 
int
main(int argc, char** argv)
{std::string fileNameWithNoExtension = "q2";
std::string graphicsFileName        = fileNameWithNoExtension + ".png";
std::string plotFileName            = fileNameWithNoExtension + ".plt";
std::string plotTitle               = "q2 plot";
std::string dataTitle               = "q2 plot";

// Instantiate the plot and set its title.
Gnuplot plot (graphicsFileName);
plot.SetTitle (plotTitle);

// Make the graphics file, which the plot file will create when it
// is used with Gnuplot, be a PNG file.
plot.SetTerminal ("png");

// Set the labels for each axis.
plot.SetLegend ("latency(delay)", "throughput");

// Set the range for the x axis.
plot.AppendExtra ("set xrange [0:+100]");

// Instantiate the dataset, set its title, and make the points be
// plotted along with connecting lines.
Gnuplot2dDataset dataset;
dataset.SetTitle (dataTitle);
dataset.SetStyle (Gnuplot2dDataset::LINES_POINTS);
	Time::SetResolution(Time::NS);
    for(double latency=0; latency<=100;latency++){
    
   // double latency_value = 0; 
    //CommandLine cmd(_FILE_);
    //cmd.Parse(argc, argv);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    NodeContainer nodes;
    nodes.Create(2);
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("7Mbps"));
    pointToPoint.SetChannelAttribute("Delay",TimeValue (MilliSeconds (latency)));
 
    NetDeviceContainer devices;
    devices = pointToPoint.Install(nodes);
 
    InternetStackHelper stack;
    stack.Install(nodes);
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
 
    Ipv4InterfaceContainer interfaces = address.Assign(devices);
 
    UdpEchoServerHelper echoServer(9);
 
    ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));
 
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));
 
    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));
    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();
    Simulator::Stop (Seconds(11.0));
    Simulator::Run();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
    std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats ();
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i){
    //Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
    //
double through_put = i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024;
    std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps\n";
    dataset.Add (latency, through_put);
    }
    flowMonitor->SerializeToXmlFile("test2.xml", true, true);
    Simulator::Destroy();
    }

// Add the dataset to the plot.
plot.AddDataset (dataset);

// Open the plot file.
std::ofstream plotFile (plotFileName.c_str());

// Write the plot file.
plot.GenerateOutput (plotFile);

// Close the plot file.
plotFile.close ();

    return 0;
}
