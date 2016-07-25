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



  /*
   * The RX/TX and link parameters can be found in:
   * "Preliminary Results of Terabit-per-second Long-Range Free-Space Optical Transmission Experiment THRUST"
   * and
   * "Overview of the Laser Communication System for the NICT Optical Ground Station and Laser Communication Experiments on Ground-to-Satellite Links"
   */
int 
main (int argc, char *argv[])
{
  bool verbose = true;

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);

  cmd.Parse (argc,argv);

  LogComponentEnable ("FsoChannel", LOG_LEVEL_INFO);
  LogComponentEnable ("FsoDownLinkPhy", LOG_LEVEL_INFO);
  LogComponentEnable ("FsoFreeSpaceLossModel", LOG_LEVEL_INFO);
  LogComponentEnable ("FsoMeanIrradianceModel", LOG_LEVEL_INFO);
  LogComponentEnable ("FsoDownLinkErrorModel", LOG_LEVEL_INFO);

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
  scintIndexModel->SetGndRefractiveIdx (1.7*10e-14);
  
  Ptr<FsoMeanIrradianceModel> meanIrradianceModel = CreateObject<FsoMeanIrradianceModel> ();

  //Channel
  Ptr<FsoChannel> channel = CreateObject<FsoChannel> ();
  channel->SetPropagationDelayModel(delayModel);

  //Error Model
  Ptr<FsoDownLinkErrorModel> errorModel = CreateObject<FsoDownLinkErrorModel> ();

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
  rxPhy->SetErrorModel (errorModel);
  
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
  double speedOfLight = 3e8;//m/s
  Ptr<Packet> packet = Create<Packet> (size);

  FsoSignalParameters params;
  params.wavelength = wavelength;
  params.frequency = speedOfLight/wavelength;
  params.symbolPeriod = 1/(49.3724e6);//49.3724 Mbps 
  params.power = 0.0;
  params.txPhy = txPhy;
  params.txAntenna = laser;
  params.txBeamwidth = 0.120;
  params.txPhaseFrontRadius = 10e8;

  txPhy->SendPacket (packet, params);
  

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}


