/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"

using namespace ns3;


int 
main (int argc, char *argv[])
{
  bool verbose = true;

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);

  cmd.Parse (argc,argv);

  
  
/*
NodeContainer c;
  c.Create (2);

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));//OGS
  positionAlloc->Add (Vector (0.0, 0.0, 46000.0));//GEO satellite
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  mobility.Install (c);

  Ptr<FsoPropagationLossModel> fsoPropLoss = CreateObject<FsoPropagationLossModel> ();
  fsoPropLoss->SetRmsWindSpeed(21.0);
  fsoPropLoss->SetGndRefractiveIdx(10e-7);

  Ptr<LaserAntennaModel> laserTransmitter = CreateObject<LaserAntennaModel> ();
  Ptr<LaserAntennaModel> laserTransmitter = CreateObject<LaserAntennaModel> ();

  Ptr<ConstantSpeedPropagationDelayModel> delayModel = Createobject<ConstantSpeedPropagationDelayModel> ();

  Ptr<FsoSingleModelSpectrumChannel> fsoChannel = Createobject<FsoSingleModelSpectrumChannel> ();
  fsoChannel->
*/
  /* ... */

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}


