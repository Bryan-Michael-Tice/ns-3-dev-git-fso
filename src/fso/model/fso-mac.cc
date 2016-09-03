/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *
 * Modified by: Michael Di Perna <diperna.michael@gmail.com> 2016
 */

#include "fso-mac.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/log.h"
#include "ns3/drop-tail-queue.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FsoMac");
NS_OBJECT_ENSURE_REGISTERED (FsoMac);

FsoMac::FsoMac ()
{
  m_txQueue = CreateObject<DropTailQueue> ();
}

FsoMac::~FsoMac ()
{
}

TypeId 
FsoMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoMac")
    .SetParent<Object> ()
    .SetGroupName ("Fso")
    .AddConstructor<FsoMac> ()
  ;
  return tid;
}

void
FsoMac::SetAddress (Mac48Address address)
{
  NS_LOG_FUNCTION (this << address);
  m_address = address;
}

Mac48Address
FsoMac::GetAddress (void) const
{
  return m_address;
}

void
FsoMac::SetFsoPhy (Ptr<FsoPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  m_phy = phy;
}

Ptr<FsoPhy> 
FsoMac::GetFsoPhy (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy;
}

void
FsoMac::SetPromisc (void)
{
  
}

void
FsoMac::SetForwardUpCallback (ForwardUpCallback upCallback)
{
  NS_LOG_FUNCTION (this);
  m_forwardUp = upCallback;
}

void
FsoMac::SetLinkUpCallback (Callback<void> linkUp)
{
  NS_LOG_FUNCTION (this);
  m_linkUp = linkUp;
}

void
FsoMac::SetLinkDownCallback (Callback<void> linkDown)
{
  NS_LOG_FUNCTION (this);
  m_linkDown = linkDown;
}

void
FsoMac::Enqueue (Ptr</*const*/ Packet> packet, Mac48Address to, Mac48Address from)
{
  NS_LOG_FUNCTION (this << packet << to << from);
  m_txQueue->Enqueue (Create<QueueItem> (packet));
}

void
FsoMac::Enqueue (Ptr</*const*/ Packet> packet, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << to);
  //We're sending this packet with a from address that is our own. We
  //get that address from the lower MAC and make use of the
  //from-spoofing Enqueue() method to avoid duplicated code.
  Enqueue (packet, to, m_address);
}

void
FsoMac::NotifyTx (Ptr<const Packet> packet)
{
  m_macTxTrace (packet);
}

void
FsoMac::NotifyTxDrop (Ptr<const Packet> packet)
{
  m_macTxDropTrace (packet);
}

void
FsoMac::NotifyRx (Ptr<const Packet> packet)
{
  m_macRxTrace (packet);
}

void
FsoMac::NotifyPromiscRx (Ptr<const Packet> packet)
{
  m_macPromiscRxTrace (packet);
}

void
FsoMac::NotifyRxDrop (Ptr<const Packet> packet)
{
  m_macRxDropTrace (packet);
}

Ptr<Packet> 
FsoMac::ForwardDown ()
{
  if (!(m_txQueue->IsEmpty ()))
   {
     return (m_txQueue->Dequeue ())->GetPacket ();
   }

  return 0;
}


} // namespace ns3

