/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

#include "fso-net-device.h"
#include "fso-mac.h"
#include "fso-phy.h"
#include "fso-channel.h"
#include "ns3/llc-snap-header.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/node.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FsoNetDevice");

NS_OBJECT_ENSURE_REGISTERED (FsoNetDevice);

TypeId
FsoNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<FsoNetDevice> ()
    .SetGroupName ("Fso")
    .AddAttribute ("Channel", "The channel attached to this device",
                   PointerValue (),
                   MakePointerAccessor (&FsoNetDevice::DoGetChannel),
                   MakePointerChecker<FsoChannel> ())
    .AddAttribute ("Phy", "The PHY layer attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&FsoNetDevice::GetPhy,
                                        &FsoNetDevice::SetPhy),
                   MakePointerChecker<FsoPhy> ())
    .AddAttribute ("Mac", "The MAC layer attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&FsoNetDevice::GetMac,
                                        &FsoNetDevice::SetMac),
                   MakePointerChecker<FsoMac> ())
  ;
  return tid;
}

FsoNetDevice::FsoNetDevice ()
  : m_mtu (1024), m_configComplete (false)
{
  NS_LOG_FUNCTION (this);
}

FsoNetDevice::~FsoNetDevice ()
{
  NS_LOG_FUNCTION (this);
}

void
FsoNetDevice::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_mac->Dispose ();
  m_phy->Dispose ();
  m_mac = 0;
  m_phy = 0;
  NetDevice::DoDispose ();
}

void
FsoNetDevice::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
  m_phy->Initialize ();
  m_mac->Initialize ();
  NetDevice::DoInitialize ();
}

void
FsoNetDevice::CompleteConfig (void)
{
  NS_LOG_FUNCTION (this);

  if (m_mac == 0
      || m_phy == 0
      || m_node == 0
      || m_configComplete)
    {
      return;
    }

  m_mac->SetFsoPhy (m_phy);
  m_mac->SetForwardUpCallback (MakeCallback (&FsoNetDevice::ForwardUp, this));
  m_mac->SetLinkUpCallback (MakeCallback (&FsoNetDevice::LinkUp, this));
  m_mac->SetLinkDownCallback (MakeCallback (&FsoNetDevice::LinkDown, this));
  m_configComplete = true;
}

void
FsoNetDevice::SetMac (Ptr<FsoMac> mac)
{
  NS_LOG_FUNCTION (this);
  m_mac = mac;
  CompleteConfig ();
}

void
FsoNetDevice::SetPhy (Ptr<FsoPhy> phy)
{
  NS_LOG_FUNCTION (this);
  m_phy = phy;
  CompleteConfig ();
}

Ptr<FsoMac>
FsoNetDevice::GetMac (void) const
{
  NS_LOG_FUNCTION (this);
  return m_mac;
}

Ptr<FsoPhy>
FsoNetDevice::GetPhy (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy;
}

void
FsoNetDevice::SetIfIndex (const uint32_t index)
{
  NS_LOG_FUNCTION (this);
  m_ifIndex = index;
}

uint32_t
FsoNetDevice::GetIfIndex (void) const
{
  NS_LOG_FUNCTION (this);
  return m_ifIndex;
}

Ptr<Channel>
FsoNetDevice::GetChannel (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy->GetChannel ();
}

Ptr<FsoChannel>
FsoNetDevice::DoGetChannel (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy->GetChannel ();
}

void
FsoNetDevice::SetAddress (Address address)
{
  NS_LOG_FUNCTION (this);
  m_mac->SetAddress (Mac48Address::ConvertFrom (address));
}

Address
FsoNetDevice::GetAddress (void) const
{
  NS_LOG_FUNCTION (this);
  return m_mac->GetAddress ();
}

bool
FsoNetDevice::IsLinkUp (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy != 0 && m_linkUp;
}

void
FsoNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
  NS_LOG_FUNCTION (this);
  m_linkChanges.ConnectWithoutContext (callback);
}

bool
FsoNetDevice::IsBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

Address
FsoNetDevice::GetBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  return Mac48Address::GetBroadcast ();
}

bool
FsoNetDevice::IsMulticast (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

Address
FsoNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  NS_LOG_FUNCTION (this);
  return Mac48Address::GetMulticast (multicastGroup);
}

Address FsoNetDevice::GetMulticast (Ipv6Address addr) const
{
  NS_LOG_FUNCTION (this);
  return Mac48Address::GetMulticast (addr);
}

bool
FsoNetDevice::IsPointToPoint (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

bool
FsoNetDevice::IsBridge (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

bool
FsoNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << dest << protocolNumber);
  NS_ASSERT (Mac48Address::IsMatchingType (dest));

  Mac48Address realTo = Mac48Address::ConvertFrom (dest);

  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  packet->AddHeader (llc);

  m_mac->NotifyTx (packet);
  m_mac->Enqueue (packet, realTo);
  
  return true;
}

Ptr<Node>
FsoNetDevice::GetNode (void) const
{
  NS_LOG_FUNCTION (this);
  return m_node;
}

void
FsoNetDevice::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this);
  m_node = node;
  CompleteConfig ();
}

bool
FsoNetDevice::NeedsArp (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

void
FsoNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  NS_LOG_FUNCTION (this);
  m_forwardUp = cb;
}

void
FsoNetDevice::ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to)
{
  NS_LOG_FUNCTION (this);
  
  LlcSnapHeader llc;
  enum NetDevice::PacketType type;
  if (to.IsBroadcast ())
    {
      type = NetDevice::PACKET_BROADCAST;
    }
  else if (to.IsGroup ())
    {
      type = NetDevice::PACKET_MULTICAST;
    }
  else if (to == m_mac->GetAddress ())
    {
      type = NetDevice::PACKET_HOST;
    }
  else
    {
      type = NetDevice::PACKET_OTHERHOST;
    }

  if (type != NetDevice::PACKET_OTHERHOST)
    {
      m_mac->NotifyRx (packet);
      packet->RemoveHeader (llc);
      m_forwardUp (this, packet, llc.GetType (), from);
    }
  else
    {
      packet->RemoveHeader (llc);
    }
    
  if (!m_promiscRx.IsNull ())
    {
      m_mac->NotifyPromiscRx (packet);
      m_promiscRx (this, packet, llc.GetType (), from, to, type);
    }
  
}

void
FsoNetDevice::LinkUp (void)
{
  NS_LOG_FUNCTION (this);
  m_linkUp = true;
  m_linkChanges ();
}

void
FsoNetDevice::LinkDown (void)
{
  NS_LOG_FUNCTION (this);
  m_linkUp = false;
  m_linkChanges ();
}

bool
FsoNetDevice::SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this);
  
  NS_ASSERT (Mac48Address::IsMatchingType (dest));
  NS_ASSERT (Mac48Address::IsMatchingType (source));

  Mac48Address realTo = Mac48Address::ConvertFrom (dest);
  Mac48Address realFrom = Mac48Address::ConvertFrom (source);

  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  packet->AddHeader (llc);

  m_mac->NotifyTx (packet);
  m_mac->Enqueue (packet, realTo, realFrom);
  
  return true;
}

void
FsoNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  NS_LOG_FUNCTION (this);
  m_promiscRx = cb;
  m_mac->SetPromisc ();
}

bool
FsoNetDevice::SupportsSendFrom (void) const
{
  NS_LOG_FUNCTION (this);
  return false; //m_mac->SupportsSendFrom ();
}

bool 
FsoNetDevice::SetMtu (const uint16_t mtu)
{
  NS_LOG_FUNCTION (this);
  m_mtu = mtu;
  return true;
}

uint16_t 
FsoNetDevice::GetMtu (void) const
{
  NS_LOG_FUNCTION (this);
  return m_mtu;
}

} //namespace ns3
