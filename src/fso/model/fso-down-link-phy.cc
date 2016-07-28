#include "fso-down-link-phy.h"
#include "fso-channel.h"
#include "fso-error-model.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/pointer.h"
#include "ns3/net-device.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/boolean.h"
#include "ns3/node.h"
#include <cmath>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FsoDownLinkPhy");

NS_OBJECT_ENSURE_REGISTERED (FsoDownLinkPhy);

TypeId
FsoDownLinkPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoDownLinkPhy")
    .SetParent<FsoPhy> ()
    .SetGroupName ("Fso")
    .AddConstructor<FsoDownLinkPhy> ()
  ;
  return tid;
}

FsoDownLinkPhy::FsoDownLinkPhy ()
{
  NS_LOG_FUNCTION (this);
  //m_random = CreateObject<UniformRandomVariable> ();
  //m_state = CreateObject<WifiPhyStateHelper> ();
}

FsoDownLinkPhy::~FsoDownLinkPhy ()
{
  NS_LOG_FUNCTION (this);
}

void
FsoDownLinkPhy::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_channel = 0;
  m_device = 0;
  m_mobility = 0;
  m_errorModel = 0;
  //m_state = 0;
}


void 
FsoDownLinkPhy::SetDevice (Ptr<NetDevice> d)
{
  //NS_ASSERT (d != 0);
  m_device = d;
}

Ptr<NetDevice> 
FsoDownLinkPhy::GetDevice () const
{
  return m_device;
}
   
void 
FsoDownLinkPhy::SetMobility (Ptr<MobilityModel> m)
{
  m_mobility = m;
}
   
Ptr<MobilityModel> 
FsoDownLinkPhy::GetMobility () const
{
  return m_mobility;
}
   
void 
FsoDownLinkPhy::SetChannel (Ptr<FsoChannel> c)
{
  m_channel = c;
}
   
Ptr<FsoChannel> 
FsoDownLinkPhy::GetChannel () const
{
  return m_channel;
}
 

Ptr<LaserAntennaModel> 
FsoDownLinkPhy::GetTxAntenna () const
{
  return m_txAntenna;
}
  
Ptr<OpticalRxAntennaModel> 
FsoDownLinkPhy::GetRxAntenna () const
{
  return m_rxAntenna;
}

void 
FsoDownLinkPhy::SetAntennas (Ptr<LaserAntennaModel> txAntenna, Ptr<OpticalRxAntennaModel> rxAntenna)
{
  m_txAntenna = txAntenna;
  m_rxAntenna = rxAntenna;
}

void 
FsoDownLinkPhy::SetErrorModel (Ptr<FsoErrorModel> errModel)
{
  m_errorModel = errModel;
}

void 
FsoDownLinkPhy::SetBitRate (double bitRate)
{
  m_bitRate = bitRate;
}

double 
FsoDownLinkPhy::GetBitRate () const
{
  return m_bitRate;
}

void 
FsoDownLinkPhy::SendPacket (Ptr<const Packet> packet, FsoSignalParameters fsoSignalParams)
{

  NS_LOG_FUNCTION (this << packet << fsoSignalParams.wavelength << fsoSignalParams.frequency);
  m_state = State::TX;
  Time txDuration = CalculateTxDuration (packet->GetSize (), fsoSignalParams);
  fsoSignalParams.power = 10*std::log10(GetTxAntenna ()->GetTxPower ()) + GetTxAntenna ()->GetGain ();  

    //m_state->SwitchToTx (txDuration, packet, GetPowerDbm (txVector.GetTxPowerLevel ()), txVector, preamble);
  
  NS_LOG_DEBUG ("PhySend: power=" << fsoSignalParams.power << "dB"); 
  m_channel->Send (this, packet, fsoSignalParams, txDuration);

}
   
void 
FsoDownLinkPhy::ReceivePacket (Ptr<Packet> packet, FsoSignalParameters fsoSignalParams)
{
  //Error model here, currently just returning the irradiance at the receiver
  NS_ASSERT (m_errorModel != 0);
  double rxIrradiance = m_errorModel->GetChunkSuccessRate(fsoSignalParams, packet->GetSize());
  double rxApertureDiameter = GetRxAntenna ()->GetApertureDiameter ();
  double rxGain = GetRxAntenna ()->GetRxGain ();
    
  double rxMeanPowerMilliWatts = (0.125*M_PI*std::pow(rxApertureDiameter,2.0)*fsoSignalParams.meanIrradiance)*1000.0;

  double rxMeanPower = 10*std::log10(0.125*M_PI*std::pow(rxApertureDiameter,2.0)*fsoSignalParams.meanIrradiance);//rxIrradiance; //This is the mean RX power now, not the instantaneous (dB)

  NS_LOG_DEBUG ("PhyReceive: incoming signal power=" << fsoSignalParams.power << "dB"); 
  NS_LOG_DEBUG ("PhyReceive: rx gain=" << rxGain << "dB");
  NS_LOG_DEBUG ("PhyReceive: mean power=" << rxMeanPower << "dB");
  
  NS_LOG_DEBUG ("PhyReceive: irradiance=" << rxIrradiance << "W/m^2, mean RX=" << rxMeanPowerMilliWatts << "mW, total RX mean power=" << (fsoSignalParams.power + rxMeanPower + rxGain) << "dB");  
}

Time 
FsoDownLinkPhy::CalculateTxDuration (uint32_t size, FsoSignalParameters fsoSignalParams)
{
  NS_ASSERT (fsoSignalParams.symbolPeriod > 0.0);
  return NanoSeconds (size * fsoSignalParams.symbolPeriod);
}



}
