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
  ;
  return tid;
}

FsoChannel::FsoChannel ()
{
}

FsoChannel::~FsoChannel ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_phyList.clear ();
  m_lossList.clear ();
}

void
FsoChannel::AddFsoPropagationLossModel (Ptr<FsoPropagationLossModel> loss)
{
  NS_ASSERT (loss != 0);
  m_lossList.push_back (loss);
}

void
FsoChannel::SetPropagationDelayModel (Ptr<PropagationDelayModel> delay)
{
  NS_ASSERT (delay != 0);
  m_delay = delay;
}

void
FsoChannel::Send (Ptr<FsoPhy> sender, Ptr<const Packet> packet,
                       FsoSignalParameters fsoSignalParams, Time duration)
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

          for (LossList::const_iterator u = m_lossList.begin (); u != m_lossList.end (); u++)
            {
              (*u)->UpdateSignalParams (fsoSignalParams, senderMobility, receiverMobility);  
            }

          /*NS_LOG_DEBUG ("propagation: txPower=" << txPowerDbm << "dbm, rxPower=" << rxPowerDbm << "dbm, " <<
                        "distance=" << senderMobility->GetDistanceFrom (receiverMobility) << "m, delay=" << delay);*/
          Ptr<Packet> copy = packet->Copy ();
          Ptr<Object> dstNetDevice = m_phyList[j]->GetDevice ();
          uint32_t dstNode;
          if (dstNetDevice == 0)
            {
              dstNode = 0xffffffff;
            }
          else
            {
              dstNode = dstNetDevice->GetObject<NetDevice> ()->GetNode ()->GetId ();
            }

          /*
          struct Parameters parameters;
          parameters.rxPowerDbm = rxPowerDbm;
          parameters.type = mpdutype;
          parameters.duration = duration;
          parameters.txVector = txVector;
          parameters.preamble = preamble;

          Simulator::ScheduleWithContext (dstNode,
                                          delay, &FsoChannel::Receive, this,
                                          j, copy, parameters);*/
          NS_LOG_DEBUG ("Scheduling Packet: " << packet->GetSize());
          Simulator::ScheduleWithContext (dstNode,
                                          delay, &FsoChannel::Receive, this,
                                          j, copy, fsoSignalParams);          
        }
    }
}

void
FsoChannel::Receive (uint32_t i, Ptr<Packet> packet, FsoSignalParameters fsoSignalParams/*struct Parameters parameters*/) const
{
  //FsoPhy receive goes here
          //m_phyList[i]->StartReceivePreambleAndHeader (packet, parameters.rxPowerDbm, parameters.txVector, parameters.preamble, parameters.type, parameters.duration);
  NS_LOG_INFO ("Channel is forwarding packet...");
  m_phyList[i]->ReceivePacket (packet, fsoSignalParams);
}

uint32_t
FsoChannel::GetNDevices (void) const
{
  return m_phyList.size ();
}

Ptr<NetDevice>
FsoChannel::GetDevice (uint32_t i) const
{
  return m_phyList[i]->GetDevice ()->GetObject<NetDevice> ();
}

void
FsoChannel::Add (Ptr<FsoPhy> phy)
{
  m_phyList.push_back (phy);
}

int64_t
FsoChannel::AssignStreams (int64_t stream) //MDP - for random variables, see header file
{
  //int64_t currentStream = stream;
  //currentStream += m_loss->AssignStreams (stream);
  //return (currentStream - stream);
  return stream;
}

} //namespace ns3
