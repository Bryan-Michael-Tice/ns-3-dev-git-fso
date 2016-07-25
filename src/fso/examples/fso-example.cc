/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/laser-antenna-model.h"
#include "ns3/optical-rx-antenna-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/fso-channel.h"
#include "ns3/fso-down-link-phy.h"
#include "ns3/fso-propagation-loss-model.h"
#include "ns3/fso-free-space-loss-model.h"
#include "ns3/fso-down-link-scintillation-index-model.h"
#include "ns3/fso-mean-irradiance-model.h"
#include <ns3/object.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FsoChannelExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);

  cmd.Parse (argc,argv);

    LogComponentEnable ("FsoChannel", LOG_LEVEL_INFO);
  
  /*
  NodeContainer c;
  c.Create (2);

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));//OGS
  positionAlloc->Add (Vector (0.0, 0.0, 36000000.0));//GEO satellite
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  mobility.Install (c);
  */

  std::cout << "Starting Fso Channel Test..." << std::endl;

  //Mobility Models
  Ptr<MobilityModel> txMobility = CreateObject<ConstantPositionMobilityModel> ();
  txMobility->SetPosition (Vector (0.0, 0.0, 36000000.0));

  Ptr<MobilityModel> rxMobility = CreateObject<ConstantPositionMobilityModel> ();
  rxMobility->SetPosition (Vector (0.0, 0.0, 0.0));

  //Antennas
  Ptr<LaserAntennaModel> laser = CreateObject<LaserAntennaModel> ();
  laser->SetBeamwidth (0.120);
  laser->SetPhaseFrontRadius (10e8);
  laser->SetOrientation (0.0);

  Ptr<OpticalRxAntennaModel> receiver = CreateObject<OpticalRxAntennaModel> ();
  receiver->SetApertureSize (0.2);
  receiver->SetRxGain (0.0);
  receiver->SetOrientation (0.0);  

  //Delay Model
  Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel> ();
  delayModel->SetSpeed (3e8);  

  //Propagation Loss Models
  Ptr<FsoFreeSpaceLossModel> freeSpaceLoss = CreateObject<FsoFreeSpaceLossModel> ();

  Ptr<FsoDownLinkScintillationIndexModel> scintIndexModel = CreateObject<FsoDownLinkScintillationIndexModel> ();
  scintIndexModel->SetRmsWindSpeed (21.0);
  scintIndexModel->SetGndRefractiveIdx (10e-7);
  
  Ptr<FsoMeanIrradianceModel> meanIrradianceModel = CreateObject<FsoMeanIrradianceModel> ();

  //Channel
  Ptr<FsoChannel> channel = CreateObject<FsoChannel> ();
  channel->SetPropagationDelayModel(delayModel);

  //Phy
  Ptr<FsoDownLinkPhy> txPhy = CreateObject<FsoDownLinkPhy> ();
  txPhy->SetMobility (txMobility);
  txPhy->SetChannel (channel);
  txPhy->SetAntenna (laser);
  txPhy->SetDevice (0);
 
  Ptr<FsoDownLinkPhy> rxPhy = CreateObject<FsoDownLinkPhy> ();
  rxPhy->SetMobility (rxMobility);
  rxPhy->SetChannel (channel);
  rxPhy->SetAntenna (receiver);
  rxPhy->SetDevice (0);
  
  //Channel Setup
  channel->SetPropagationDelayModel (delayModel);
  channel->AddFsoPropagationLossModel (freeSpaceLoss);
  channel->AddFsoPropagationLossModel (scintIndexModel);
  channel->AddFsoPropagationLossModel (meanIrradianceModel);
  channel->Add(txPhy);
  channel->Add(rxPhy);

  //Setup Packet and Signal Params
  uint32_t size = 1024;//Packet size
  double wavelength = 1550e-9; //1550 nm 
  Ptr<Packet> packet = Create<Packet> (size);
  FsoSignalParameters params;
  params.wavelength = wavelength;
  params.frequency = 3e8/wavelength;
  params.symbolPeriod = 1/(1.8e9);//1.8 Gbps 
  params.power = 0.0;
  params.txPhy = txPhy;
  params.txAntenna = laser;
  params.txBeamwidth = 0.120;
  
  txPhy->SendPacket (packet, params);
  

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}


