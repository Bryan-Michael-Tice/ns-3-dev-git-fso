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

#ifndef FSO_MAC_H
#define FSO_MAC_H

#include <ns3/object.h>
#include "ns3/packet.h"
#include "ns3/mac48-address.h"
#include "ns3/drop-tail-queue.h"
//#include "ns3/mac-low.h"
#include "fso-phy.h"
//#include "ns3/ssid.h"
//#include "ns3/qos-utils.h"

namespace ns3 {

class FsoPhy;

class FsoMac  : public Object
{

public:
  
  FsoMac ();
  virtual ~FsoMac ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \return the MAC address associated to this MAC layer.
   */
  virtual Mac48Address GetAddress (void) const;

  /**
   * \param address the current address of this MAC layer.
   */
  virtual void SetAddress (Mac48Address address);

  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   * \param from the address from which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the DCF function determines that
   * access it granted to this MAC. The extra parameter "from" allows
   * this device to operate in a bridged mode, forwarding received
   * frames without altering the source address.
   */
  virtual void Enqueue (Ptr</*const*/ Packet> packet, Mac48Address to, Mac48Address from);

  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the channel access function determines that
   * access is granted to this MAC.
   */
  virtual void Enqueue (Ptr</*const*/ Packet> packet, Mac48Address to);

  /**
   * \param phy the physical layer attached to this MAC.
   */
  virtual void SetFsoPhy (Ptr<FsoPhy> phy);

  /**
   * return current attached FsoPhy device
   */
  virtual Ptr<FsoPhy> GetFsoPhy (void) const;

  /**
   * \brief Sets the interface in promiscuous mode.
   *
   * Enables promiscuous mode on the interface. Note that any further
   * filtering on the incoming frame path may affect the overall
   * behavior.
   */
  virtual void SetPromisc (void);


  typedef Callback<void, Ptr<Packet>, Mac48Address, Mac48Address> ForwardUpCallback;

  /**
   * \param upCallback the callback to invoke when a packet must be forwarded up the stack.
   */
  virtual void SetForwardUpCallback (Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> upCallback);

  /**
   * \param linkUp the callback to invoke when the link becomes up.
   */
  virtual void SetLinkUpCallback (Callback<void> linkUp);

  /**
   * \param linkDown the callback to invoke when the link becomes down.
   */
  virtual void SetLinkDownCallback (Callback<void> linkDown);

  /**
   * \param packet the packet being enqueued
   *
   * Public method used to fire a MacTx trace. Implemented for encapsulation purposes.
   * Note this trace indicates that the packet was accepted by the device only.
   * The packet may be dropped later (e.g. if the queue is full).
   */
  void NotifyTx (Ptr<const Packet> packet);

  /**
   * \param packet the packet being dropped
   *
   * Public method used to fire a MacTxDrop trace. Implemented for encapsulation purposes.
   * This trace indicates that the packet was dropped before it was transmitted
   * (e.g. when a STA is not associated with an AP).
   */
  void NotifyTxDrop (Ptr<const Packet> packet);

  /**
   * \param packet the packet we received
   *
   * Public method used to fire a MacRx trace. Implemented for encapsulation purposes.
   */
  void NotifyRx (Ptr<const Packet> packet);

  /**
   * \param packet the packet we received promiscuously
   *
   * Public method used to fire a MacPromiscRx trace. Implemented for encapsulation purposes.
   */
  void NotifyPromiscRx (Ptr<const Packet> packet);

  /**
   * \param packet the packet we received but is not destined for us
   *
   * Public method used to fire a MacRxDrop trace. Implemented for encapsulation purposes.
   */
  void NotifyRxDrop (Ptr<const Packet> packet);

  /**
   * return next packet in queue, if queue is empty return 0
   */
  Ptr<Packet> ForwardDown ();

protected:
  Mac48Address m_address;        //!< Address of MAC
  Ptr<DropTailQueue<Packet>>  m_txQueue;  //!< Queue for outgoing packets

  Callback<void> m_linkUp;       
  Callback<void> m_linkDown;     

private:
  Ptr<FsoPhy> m_phy;
  ForwardUpCallback m_forwardUp;

  /**
   * The trace source fired when packets come into the "top" of the device
   * at the L3/L2 transition, before being queued for transmission.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macTxTrace;
  /**
   * The trace source fired when packets coming into the "top" of the device
   * are dropped at the MAC layer during transmission.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macTxDropTrace;
  /**
   * The trace source fired for packets successfully received by the device
   * immediately before being forwarded up to higher layers (at the L2/L3
   * transition).  This is a promiscuous trace.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macPromiscRxTrace;
  /**
   * The trace source fired for packets successfully received by the device
   * immediately before being forwarded up to higher layers (at the L2/L3
   * transition).  This is a non- promiscuous trace.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macRxTrace;
  /**
   * The trace source fired when packets coming into the "top" of the device
   * are dropped at the MAC layer during reception.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macRxDropTrace;
};

} // namespace ns3


#endif /* FSO_MAC_H */
