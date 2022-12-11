/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
 * Author: Mathieu Lacage, <mathieu.lacage@sophia.inria.fr>
 *
 * Modified by: Michael Di Perna <diperna.michael@gmail.com> 2016
 */

#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/object-factory.h"
#include "fso-channel.h"
#include "ns3/propagation-delay-model.h"
#include <cstdlib>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FsoChannel");

NS_OBJECT_ENSURE_REGISTERED (FsoChannel);

TypeId
FsoChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoChannel")
    .SetParent<Channel> ()
    .SetGroupName ("Fso")
    .AddConstructor<FsoChannel> ()
    .AddAttribute ("PropagationDelayModel", "A pointer to the propagation delay model attached to this channel.",
                   PointerValue (),
                   MakePointerAccessor (&FsoChannel::m_delay),
                   MakePointerChecker<PropagationDelayModel> ())
    .AddAttribute ("FsoPropagationLossModel", "A pointer to the fso propagation loss model attached to this channel.",
                   PointerValue (),
                   MakePointerAccessor (&FsoChannel::m_loss),
                   MakePointerChecker<FsoPropagationLossModel> ())
  ;
  return tid;
}

FsoChannel::FsoChannel ()
{
  NS_LOG_FUNCTION (this);
}

FsoChannel::~FsoChannel ()
{
  NS_LOG_FUNCTION (this);
  m_phyList.clear ();
}

void
FsoChannel::AddFsoPropagationLossModel (Ptr<FsoPropagationLossModel> loss)
{
  NS_ASSERT (loss != 0);
  m_loss = loss;
}

void
FsoChannel::SetPropagationDelayModel (Ptr<PropagationDelayModel> delay)
{
  NS_ASSERT (delay != 0);
  m_delay = delay;
}

void
FsoChannel::Send (Ptr<FsoPhy> sender, Ptr<const Packet> packet,
                       Ptr<FsoSignalParameters> fsoSignalParams, Time duration)
{
  Ptr<MobilityModel> senderMobility = sender->GetMobility ()->GetObject<MobilityModel> ();
  NS_ASSERT (senderMobility != 0);
  uint32_t j = 0;
  for (PhyList::const_iterator i = m_phyList.begin (); i != m_phyList.end (); i++, j++)
    {
      if (sender != (*i))
        {

          Ptr<MobilityModel> receiverMobility = (*i)->GetMobility ()->GetObject<MobilityModel> ();
          Time delay = m_delay->GetDelay (senderMobility, receiverMobility);
          double txPower = fsoSignalParams->power;

          m_loss->UpdateSignalParams (fsoSignalParams, senderMobility, receiverMobility);  

          NS_LOG_DEBUG ("Signal Channel: txPower=" << txPower << "db, distance=" << senderMobility->GetDistanceFrom (receiverMobility) << "m, delay=" << delay << ", Scint Index=" << fsoSignalParams->scintillationIndex << ", mean irradiance=" << fsoSignalParams->meanIrradiance << "W/m^2, path loss=" << fsoSignalParams->pathLoss <<"db, power after FSPL" << fsoSignalParams->power << "dB");

          Ptr<Packet> copy = packet->Copy ();
          Ptr<FsoNetDevice> dstNetDevice = m_phyList[j]->GetFsoDevice ();
          uint32_t dstNode;
          if (dstNetDevice == 0)
            {
              dstNode = 0xffffffff;
            }
          else
            {
              dstNode = dstNetDevice->GetObject<NetDevice> ()->GetNode ()->GetId ();
            }

          NS_LOG_DEBUG ("Scheduling Packet: " << packet->GetSize() << " bytes"); 
          Simulator::ScheduleWithContext (dstNode,
                                          delay, &FsoPhy::Receive, m_phyList[j],
                                          copy, fsoSignalParams);                 
        }
    }
}

std::size_t
FsoChannel::GetNDevices (void) const
{
  return m_phyList.size ();
}

Ptr<NetDevice>
//FsoChannel::GetDevice::std::(size_t) i const
FsoChannel::GetDevice (std::size_t i) const
//FsoChannel::GetDevice std::size_t i const 
{
  return m_phyList[i]->GetFsoDevice ()->GetObject<NetDevice> ();
}

void
FsoChannel::Add (Ptr<FsoPhy> phy)
{
  m_phyList.push_back (phy);
}

int64_t
FsoChannel::AssignStreams (int64_t stream)
{
  int64_t currentStream = stream;
  currentStream += m_loss->AssignStreams (stream);
  return (currentStream - stream);
}

} //namespace ns3
