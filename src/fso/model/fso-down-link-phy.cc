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
   
Ptr<AntennaModel> 
FsoDownLinkPhy::GetRxAntenna () const
{
  return m_antenna;
}

void 
FsoDownLinkPhy::SetAntenna (Ptr<AntennaModel> antenna)
{
  m_antenna = antenna;
}

Ptr<AntennaModel> 
FsoDownLinkPhy::GetAntenna () const
{
  return m_antenna;
}

void 
FsoDownLinkPhy::SendPacket (Ptr<const Packet> packet, FsoSignalParameters fsoSignalParams)
{

  NS_LOG_FUNCTION (this << packet << fsoSignalParams.wavelength << fsoSignalParams.frequency);

  m_state = State::TX;
  Time txDuration = CalculateTxDuration (packet->GetSize (), fsoSignalParams);  

    //m_state->SwitchToTx (txDuration, packet, GetPowerDbm (txVector.GetTxPowerLevel ()), txVector, preamble);

  m_channel->Send (this, packet, fsoSignalParams, txDuration);

}
   
void 
FsoDownLinkPhy::ReceivePacket (Ptr<Packet> packet, FsoSignalParameters fsoSignalParams)
{
  //Error model here
}

Time 
FsoDownLinkPhy::CalculateTxDuration (uint32_t size, FsoSignalParameters fsoSignalParams)
{
  NS_ASSERT (fsoSignalParams.symbolPeriod > 0.0);
  return NanoSeconds (size * fsoSignalParams.symbolPeriod);
}



}
