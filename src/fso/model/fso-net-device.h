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

#ifndef FSO_NET_DEVICE_H
#define FSO_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/packet.h"
#include "ns3/traced-callback.h"
#include "ns3/mac48-address.h"
#include "fso-mac.h"
#include <string>

namespace ns3 {

class FsoChannel;
class FsoPhy;
class FsoMac;


/**
 * \brief Hold together all fso related objects.
 * \ingroup fso
 *
 * This class holds together ns3::FsoChannel, ns3::FsoPhy,
 *  and ns3::FsoMac.
 */
class FsoNetDevice : public NetDevice
{
public:
  static TypeId GetTypeId (void);

  FsoNetDevice ();
  virtual ~FsoNetDevice ();

  /**
   * \param mac the mac layer to use.
   */
  void SetMac (Ptr<FsoMac> mac);

  /**
   * \param phy the phy layer to use.
   */
  void SetPhy (Ptr<FsoPhy> phy);

  /**
   * \returns the mac we are currently using.
   */
  Ptr<FsoMac> GetMac (void) const;

  /**
   * \returns the phy we are currently using.
   */
  Ptr<FsoPhy> GetPhy (void) const;

  //inherited from NetDevice base class.
  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex (void) const;
  virtual Ptr<Channel> GetChannel (void) const;
  virtual void SetAddress (Address address);
  virtual Address GetAddress (void) const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;
  virtual bool IsLinkUp (void) const;
  virtual void AddLinkChangeCallback (Callback<void> callback);
  virtual bool IsBroadcast (void) const;
  virtual Address GetBroadcast (void) const;
  virtual bool IsMulticast (void) const;
  virtual Address GetMulticast (Ipv4Address multicastGroup) const;
  virtual bool IsPointToPoint (void) const;
  virtual bool IsBridge (void) const;
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);

  virtual Address GetMulticast (Ipv6Address addr) const;

  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
  virtual bool SupportsSendFrom (void) const;

protected:
  virtual void DoDispose (void);
  virtual void DoInitialize (void);

  /**
   * Receive a packet from the lower layer and pass the
   * packet up the stack.
   *
   * \param packet
   * \param from
   * \param to
   */
  void ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to);


private:

  /**
   * Set that the link is up.
   */
  void LinkUp (void);

  /**
   * Set that the link is down.
   */
  void LinkDown (void);

  /**
   * Return the FsoChannel this device is connected to.
   *
   * \return FsoChannel
   */
  Ptr<FsoChannel> DoGetChannel (void) const;
  /**
   * Complete the configuration of this fso device by
   * connecting all lower components (e.g. MAC) together.
   */
  void CompleteConfig (void);

  Ptr<Node> m_node;
  Ptr<FsoPhy> m_phy;
  Ptr<FsoMac> m_mac;
  NetDevice::ReceiveCallback m_forwardUp;
  NetDevice::PromiscReceiveCallback m_promiscRx;

  TracedCallback<Ptr<const Packet>, Mac48Address> m_rxLogger;
  TracedCallback<Ptr<const Packet>, Mac48Address> m_txLogger;

  uint32_t m_ifIndex;
  bool m_linkUp;
  TracedCallback<> m_linkChanges;
  uint16_t m_mtu;
  bool m_configComplete;
};

} //namespace ns3

#endif /* FSO_NET_DEVICE_H */
