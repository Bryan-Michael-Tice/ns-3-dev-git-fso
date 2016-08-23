/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 *
 * Modified by: Michael Di Perna <diperna.michael@gmail.com> 2016
 */

#include "fso-phy.h"
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

NS_LOG_COMPONENT_DEFINE ("FsoPhy");

NS_OBJECT_ENSURE_REGISTERED (FsoPhy);

TypeId
FsoPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoPhy")
    .SetParent<Object> ()
    .SetGroupName ("Fso")
    .AddConstructor<FsoPhy> ()
    .AddAttribute ("BitRate",
                   "Phy bit rate in bps.",
                   DoubleValue (49.3724e6),
                   MakeDoubleAccessor (&FsoPhy::SetBitRate,
                                       &FsoPhy::GetBitRate),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

FsoPhy::FsoPhy () : m_txState (State::IDLE)
{
  NS_LOG_FUNCTION (this);
  m_txDurationTimer.SetFunction (&FsoPhy::SwitchToIdle, this);
  
}

FsoPhy::~FsoPhy ()
{
  NS_LOG_FUNCTION (this);
}

void
FsoPhy::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_channel = 0;
  m_device = 0;
  m_mobility = 0;
  m_errorModel = 0;
}

void 
FsoPhy::SetReceiveOkCallback (RxOkCallback callback)
{
  m_rxOkCallback = callback; 
}

void 
FsoPhy::SetReceiveErrorCallback (RxErrorCallback callback)
{
  m_rxErrorCallback = callback; 
}

void
FsoPhy::SwitchFromRxEndOk (Ptr<Packet> packet, double snr, Ptr<FsoSignalParameters> params)
{
  //These commented functions are left for reference
  //m_rxOkTrace (packet, snr, txVector.GetMode (), preamble);
  //NotifyRxEndOk ();
  //DoSwitchFromRx ();

  if (!m_rxOkCallback.IsNull ())
    { 
      m_rxOkCallback (packet, snr, params);
    }

}

void
FsoPhy::SwitchFromRxEndError (Ptr<Packet> packet, double snr)
{
  //These commented functions are left for reference
  //m_rxErrorTrace (packet, snr);
  //NotifyRxEndError ();
  //DoSwitchFromRx ();

  if (!m_rxErrorCallback.IsNull ())
    {
      m_rxErrorCallback (packet, snr);
    }
}

void 
FsoPhy::SetDevice (Ptr<NetDevice> d)
{
  //NS_ASSERT (d != 0);
  m_device = d;
}

Ptr<NetDevice> 
FsoPhy::GetDevice () const
{
  return m_device;
}
   
void 
FsoPhy::SetMobility (Ptr<MobilityModel> m)
{
  m_mobility = m;
}
   
Ptr<MobilityModel> 
FsoPhy::GetMobility () const
{
  return m_mobility;
}
   
void 
FsoPhy::SetChannel (Ptr<FsoChannel> c)
{
  m_channel = c;
}
   
Ptr<FsoChannel> 
FsoPhy::GetChannel () const
{
  return m_channel;
}
 

Ptr<LaserAntennaModel> 
FsoPhy::GetTxAntenna () const
{
  return m_txAntenna;
}
  
Ptr<OpticalRxAntennaModel> 
FsoPhy::GetRxAntenna () const
{
  return m_rxAntenna;
}

void 
FsoPhy::SetAntennas (Ptr<LaserAntennaModel> txAntenna, Ptr<OpticalRxAntennaModel> rxAntenna)
{
  m_txAntenna = txAntenna;
  m_rxAntenna = rxAntenna;
}

void 
FsoPhy::SetErrorModel (Ptr<FsoErrorModel> errModel)
{
  m_errorModel = errModel;
}

void 
FsoPhy::SetBitRate (double bitRate)
{
  m_bitRate = bitRate;
}

double 
FsoPhy::GetBitRate () const
{
  return m_bitRate;
}

void 
FsoPhy::SwitchToTx (Time duration)
{
  NS_ASSERT (m_txState == State::IDLE);
  m_txState = State::TX;
  
  //Schedule IDLE state change
  m_txDurationTimer.SetDelay (duration);
  m_txDurationTimer.Schedule ();
}

void 
FsoPhy::SwitchToIdle ()
{
  m_txState = State::IDLE;
}

FsoPhy::State
FsoPhy::GetTxState ()
{
  return m_txState;
}

void 
FsoPhy::Transmit (Ptr<const Packet> packet, Ptr<FsoSignalParameters> fsoSignalParams)
{

  NS_ASSERT (m_txState == State::IDLE);

  NS_LOG_FUNCTION (this << packet << fsoSignalParams->wavelength);
  
  Ptr<LaserAntennaModel> laser = GetTxAntenna();

  fsoSignalParams->power                = laser->GetTxPower () + laser->GetGain ();  
  fsoSignalParams->txBeamwidth          = laser->GetBeamwidth ();
  fsoSignalParams->txPhy                = this;
  fsoSignalParams->txAntenna            = laser;
  fsoSignalParams->symbolPeriod         = 1/m_bitRate;
  fsoSignalParams->wavelength           = laser->GetWavelength ();
  fsoSignalParams->frequency            = 3e8/(laser->GetWavelength ());

  Time txDuration = CalculateTxDuration (packet->GetSize (), fsoSignalParams);

  SwitchToTx (txDuration);
  
  NS_LOG_DEBUG ("PhySend: power=" << fsoSignalParams->power << "dB"); 
  m_channel->Send (this, packet, fsoSignalParams, txDuration);

}
   
void 
FsoPhy::Receive (Ptr<Packet> packet, Ptr<FsoSignalParameters> fsoSignalParams)
{
  NS_ASSERT (m_errorModel != 0);
  //double rxGain = GetRxAntenna ()->GetRxGain ();


  double packetSuccessRate = m_errorModel->GetPacketSuccessRate(packet, fsoSignalParams);
  NS_LOG_DEBUG ("PhyReceive: packet success rate=" << packetSuccessRate);

  SwitchFromRxEndOk(packet, 0.0, fsoSignalParams);//should provide SNR once the SNR function is implemented, 0.0 placeholder
}

Time 
FsoPhy::CalculateTxDuration (uint32_t size, Ptr<FsoSignalParameters> fsoSignalParams) const
{
  NS_LOG_DEBUG ("PhyTransmit: symbol period=" << fsoSignalParams->symbolPeriod << "s"); 
  NS_ASSERT (fsoSignalParams->symbolPeriod > 0.0);
  return NanoSeconds (size * 8 * fsoSignalParams->symbolPeriod);
}


}//namespace
