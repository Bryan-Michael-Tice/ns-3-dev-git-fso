/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Michael Di Perna
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Michael Di Perna <diperna.michael@gmail.com>
 */
#include "ns3/core-module.h"
#include "ns3/mobility-helper.h"
#include "ns3/node-container.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/laser-antenna-model.h"
#include "ns3/optical-rx-antenna-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/fso-channel.h"
#include "ns3/fso-phy.h"
#include "ns3/fso-propagation-loss-model.h"
#include "ns3/fso-free-space-loss-model.h"
#include "ns3/fso-down-link-scintillation-index-model.h"
#include "ns3/fso-mean-irradiance-model.h"
#include "ns3/fso-helper.h"
#include "ns3/fso-net-device.h"
#include "ns3/internet-stack-helper.h"

#include <ns3/object-factory.h>
#include "ns3/network-module.h"
#include "ns3/double.h"

#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"
#include "ns3/orchestrator.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FsoChannelHelperExample");

// Sends a single packet from a geo-stationary satellite to an optical ground station
// A large elevation angle is assumed which corresponds to weak atmospheric turbulence
//
//                                        SATELLITE
//                                             |
//                                             |
//                                             |
//                                             |
//                                             V
//                                       GROUND STATION 
//
// Use verbose option to log output from the loss models and error model
//
// The RX/TX and link parameters can be found in the following papers: "Preliminary Results of Terabit-per-second Long-Range Free-Space Optical Transmission Experiment THRUST"
// and 
// "Overview of the Laser Communication System for the NICT Optical Ground Station and Laser Communication Experiments on Ground-to-Satellite Links"
//
// The channel uses 3 loss models:
// 
// 1) Free Space Path loss - the geometric power loss due to the propagation distance and wavelength
// 2) Scintillation Index  - A parameter which characterizes the fluctuations in irradiance due to atmospheric turbulence
// 3) Mean Irradiance      - The mean irradiance at the receiver due to the increase in the beamwidth as it propagates 
//
// The error model attached to the receiver computes the received irradiance based on the scintillation index and mean irradiance


int 
main (int argc, char *argv[])
{
  LogComponentEnable ("FsoChannelHelperExample", LOG_LEVEL_INFO);

  bool verbose = true;
  bool packetsend = true;

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);
  cmd.AddValue ("packetsend", "Use Packet Socket to send packet if true", packetsend);

  cmd.Parse (argc,argv);

  Config::SetDefault ("ns3::LaserAntennaModel::Beamwidth", DoubleValue (0.06));
  Config::SetDefault ("ns3::LaserAntennaModel::Wavelength", DoubleValue (847e-9));
  Config::SetDefault ("ns3::LaserAntennaModel::TxPower", DoubleValue (-10.0));
  Config::SetDefault ("ns3::LaserAntennaModel::Gain", DoubleValue (116.0));

  Config::SetDefault ("ns3::OpticalRxAntennaModel::ReceiverGain", DoubleValue (121.4));
  Config::SetDefault ("ns3::OpticalRxAntennaModel::ApertureDiameter", DoubleValue (0.318));

  NodeContainer nodes;
  nodes.Create (2);

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();

  positionAlloc->Add (Vector (0.0, 0.0, 707000));//Satellite at 707,000 meters above the Earth
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));// Ground station at 0 meters
  mobility.SetPositionAllocator (positionAlloc);

  mobility.Install (nodes);

  FsoChannelHelper fsoChannelHelper = FsoChannelHelper::Default ();
  Ptr<FsoChannel> channel = fsoChannelHelper.Create ();

  FsoPhyHelper fsoPhyHelper = FsoPhyHelper::Default ();
  fsoPhyHelper.Set ("BitRate", DoubleValue (49.3724e6));
  fsoPhyHelper.SetChannel (channel);

  NetDeviceContainer devices;
  FsoHelper fsoHelper;
  FsoMacHelper fsoMacHelper;

  if (verbose)
   {
      LogComponentEnable ("FsoHelper", LOG_LEVEL_ALL);
      LogComponentEnable ("FsoNetDevice", LOG_LEVEL_ALL);
      LogComponentEnable ("FsoChannel", LOG_LEVEL_ALL);
      LogComponentEnable ("FsoPhy", LOG_LEVEL_ALL);
      LogComponentEnable ("FsoMac", LOG_LEVEL_ALL);
      LogComponentEnable ("FsoFreeSpaceLossModel", LOG_LEVEL_ALL); 
      LogComponentEnable ("FsoMeanIrradianceModel", LOG_LEVEL_ALL);
      LogComponentEnable ("FsoErrorModel", LOG_LEVEL_ALL);
      LogComponentEnable ("FsoDownLinkScintillationIndexModel", LOG_LEVEL_ALL);
   } else
   {
      LogComponentEnable ("FsoHelper", LOG_LEVEL_DEBUG);
      LogComponentEnable ("FsoNetDevice", LOG_LEVEL_DEBUG);
      LogComponentEnable ("FsoChannel", LOG_LEVEL_DEBUG);
      LogComponentEnable ("FsoPhy", LOG_LEVEL_DEBUG);
      LogComponentEnable ("FsoMac", LOG_LEVEL_DEBUG);
      //LogComponentEnable ("FsoFreeSpaceLossModel", LOG_LEVEL_DEBUG); 
      //LogComponentEnable ("FsoMeanIrradianceModel", LOG_LEVEL_DEBUG);
      //LogComponentEnable ("FsoErrorModel", LOG_LEVEL_DEBUG);
      //LogComponentEnable ("FsoDownLinkScintillationIndexModel", LOG_LEVEL_DEBUG);
   }

  devices = fsoHelper.Install (fsoPhyHelper, fsoMacHelper, nodes);

  Ptr<NetDevice> txNetDevice = devices.Get (0);
  Ptr<NetDevice> rxNetDevice = devices.Get (1);

  Ptr<FsoNetDevice> txDevice = txNetDevice->GetObject<FsoNetDevice> ();
  Ptr<FsoNetDevice> rxDevice = rxNetDevice->GetObject<FsoNetDevice> ();

  Ptr<FsoPhy> txPhy = txDevice->GetPhy ();
  Ptr<FsoPhy> rxPhy = rxDevice->GetPhy ();
  
  Ptr<LaserAntennaModel> laser = CreateObject<LaserAntennaModel> ();
  Ptr<OpticalRxAntennaModel> opticalRx = CreateObject<OpticalRxAntennaModel> ();

  txPhy->SetAntennas (laser, opticalRx);
  rxPhy->SetAntennas (laser, opticalRx);

   if(packetsend)
   {
    PacketSocketHelper packetSocket;
    // give packet socket powers to nodes.
    packetSocket.Install (nodes);

    PacketSocketAddress socketAddr;
    socketAddr.SetSingleDevice (txDevice->GetIfIndex ());
    socketAddr.SetPhysicalAddress (rxDevice->GetAddress ());
    // Arbitrary protocol type.
    // Note: PacketSocket doesn't have any L4 multiplexing or demultiplexing
    //       The only mux/demux is based on the protocol field
    socketAddr.SetProtocol (1);

    Ptr<PacketSocketClient> client = CreateObject<PacketSocketClient> ();
    client->SetRemote (socketAddr);
    nodes.Get (0)->AddApplication (client);

    Ptr<PacketSocketServer> server = CreateObject<PacketSocketServer> ();
    server->SetLocal (socketAddr);
    nodes.Get (1)->AddApplication (server);
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    Simulator::Stop (Seconds (10.0));
    Simulator::Run ();
    Simulator::Destroy ();

  } else  
  {  
    LogComponentEnable ("InternetStackHelper", LOG_LEVEL_ALL);
  
    InternetStackHelper stack;
    stack.Install (nodes);

    Ipv4AddressHelper ipAddressHelper;
    ipAddressHelper.SetBase("192.168.1.0", "255.255.255.0");

    Ipv4InterfaceContainer ipInterfaces = ipAddressHelper.Assign(devices);

    UdpEchoServerHelper echoServer (9);
    ApplicationContainer serverApps = echoServer.Install (nodes.Get(0));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));

  
    UdpEchoClientHelper echoClient (ipInterfaces.GetAddress(0), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (100));

    ApplicationContainer clientApps = echoClient.Install (nodes.Get(1));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
      
    Orchestrator::SetSimulationEndTime(Seconds(10));
    Orchestrator::BeginSimulation("FSO", "helper", "study");
  }
   
  return 0;
}
