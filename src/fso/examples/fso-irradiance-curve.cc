/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 CTTC
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

#include <ns3/object-factory.h>
#include "ns3/double.h"
#include "ns3/gnuplot.h"

#include <fstream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FsoIrradianceCurve");

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

Gnuplot2dDataset g_dataset;

void
PlotPacketRx (Ptr<Packet> pkt, double snr, Ptr<FsoSignalParameters> params)
{
  static uint32_t packetNumber = 1;
  g_dataset.Add (packetNumber, params->normIrradiance);
  packetNumber++;
}

void 
SetupPlot ()
{

}

int 
main (int argc, char *argv[])
{
  bool verbose = true;

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);//!!!Need to use verbose argument

  cmd.Parse (argc,argv);

  LogComponentEnable ("FsoChannel", LOG_LEVEL_INFO);
  LogComponentEnable ("FsoPhy", LOG_LEVEL_INFO);
  LogComponentEnable ("FsoFreeSpaceLossModel", LOG_LEVEL_INFO); 
  LogComponentEnable ("FsoMeanIrradianceModel", LOG_LEVEL_INFO);
  LogComponentEnable ("FsoDownLinkErrorModel", LOG_LEVEL_INFO);
  LogComponentEnable ("FsoDownLinkScintillationIndexModel", LOG_LEVEL_INFO);
  LogComponentEnable ("FsoIrradianceCurve", LOG_LEVEL_INFO);


  //Plot Setup
  std::string fileNameWithNoExtension = "normalized-irradiance";
  std::string graphicsFileName        = fileNameWithNoExtension + ".png";
  std::string plotFileName            = fileNameWithNoExtension + ".plt";
  std::string plotTitle               = "2-D Plot";
  std::string dataTitle               = "2-D Data"; 

  // Instantiate the plot and set its title.
  Gnuplot plot (graphicsFileName);
  plot.SetTitle (plotTitle);
  
  // Make the graphics file, which the plot file will create when it
  // is used with Gnuplot, be a PNG file.
  plot.SetTerminal ("png");
  
  // Set the labels for each axis.
  plot.SetLegend ("X Values", "Y Values");
   
  // Set the range for the x axis.
  plot.AppendExtra ("set xrange [0:+100]");
  
  g_dataset.SetTitle (dataTitle);
  g_dataset.SetStyle (Gnuplot2dDataset::LINES_POINTS);

  //Mobility Models
  Ptr<MobilityModel> txMobility = CreateObject<ConstantPositionMobilityModel> ();
  txMobility->SetPosition (Vector (0.0, 0.0, 707000));//Satellite at 707,000 meters above the Earth

  Ptr<MobilityModel> rxMobility = CreateObject<ConstantPositionMobilityModel> ();
  rxMobility->SetPosition (Vector (0.0, 0.0, 0.0));// Ground station at 0 meters 

  //Antennas
  Ptr<LaserAntennaModel> laser = CreateObject<LaserAntennaModel> ();
  laser->SetBeamwidth (0.06); //meters
  laser->SetOrientation (0.0);
  laser->SetTxPower (-1.0);//dB
  laser->SetGain (116.0);//dB

  ObjectFactory rxPhyFactory;
  rxPhyFactory.SetTypeId ("ns3::OpticalRxAntennaModel");
  rxPhyFactory.Set ("ReceiverGain", DoubleValue (121.4));     
  rxPhyFactory.Set ("ApertureDiameter", DoubleValue (0.318));
  Ptr<OpticalRxAntennaModel> receiver = (rxPhyFactory.Create ())->GetObject<OpticalRxAntennaModel> ();     
  receiver->SetOrientation (0.0);
  

  //Delay Model
  Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel> ();  

  //Propagation Loss Models
  Ptr<FsoFreeSpaceLossModel> freeSpaceLoss = CreateObject<FsoFreeSpaceLossModel> ();

  Ptr<FsoDownLinkScintillationIndexModel> scintIndexModel = CreateObject<FsoDownLinkScintillationIndexModel> ();
  scintIndexModel->SetRmsWindSpeed (21.0); //From the Hufnagel-Valley 5/7 model
  scintIndexModel->SetGndRefractiveIdx (1.7e-14); //From the Hufnagel-Valley 5/7 model
  
  Ptr<FsoMeanIrradianceModel> meanIrradianceModel = CreateObject<FsoMeanIrradianceModel> ();

  freeSpaceLoss->SetNext (scintIndexModel);
  scintIndexModel->SetNext (meanIrradianceModel);

  //Channel
  Ptr<FsoChannel> channel = CreateObject<FsoChannel> ();
  channel->SetPropagationDelayModel(delayModel);

  //Error Model
  Ptr<FsoDownLinkErrorModel> errorModel = CreateObject<FsoDownLinkErrorModel> ();

  //Phy
  double bitRate = 49.3724e6;
  Ptr<FsoPhy> txPhy = CreateObject<FsoPhy> ();
  txPhy->SetMobility (txMobility);
  txPhy->SetChannel (channel);
  txPhy->SetAntennas (laser, 0);
  txPhy->SetFsoDevice (0);
  txPhy->SetBitRate (bitRate);
 
  Ptr<FsoPhy> rxPhy = CreateObject<FsoPhy> ();
  rxPhy->SetMobility (rxMobility);
  rxPhy->SetChannel (channel);
  rxPhy->SetAntennas (0, receiver);
  rxPhy->SetFsoDevice (0);
  rxPhy->SetErrorModel (errorModel);
  rxPhy->SetBitRate (bitRate);
  rxPhy->SetReceiveOkCallback ( MakeCallback (&PlotPacketRx));
  errorModel->SetPhy (rxPhy);
  
  
  //Channel Setup
  channel->SetPropagationDelayModel (delayModel);
  channel->AddFsoPropagationLossModel (freeSpaceLoss);
  channel->Add(txPhy);
  channel->Add(rxPhy);

  //Setup Packet and Signal Params
  uint32_t size = 1024;//Packet size in bytes
  Ptr<Packet> packet = Create<Packet> (size);

  double packetDelay = 8.0*(22e-6); //seconds between packets to be sent
  double numPackets = 100;
  double simTime = numPackets*packetDelay + 1.0;

  Simulator::Stop (Seconds (simTime));

  for (int i = 0; i < numPackets; i++)
   {
     Simulator::Schedule (Seconds (0.1 + ((double)(i))*packetDelay), &FsoPhy::Transmit, txPhy, packet); //Send packet from transmitter Phy
   }   

  
  Simulator::Run ();
  Simulator::Destroy ();

  // Add the dataset to the plot.
  plot.AddDataset (g_dataset);
  
  // Open the plot file.
  std::ofstream plotFile (plotFileName.c_str());
  
  // Write the plot file.
  plot.GenerateOutput (plotFile);
  
  // Close the plot file.
  plotFile.close ();
  return 0;
}


