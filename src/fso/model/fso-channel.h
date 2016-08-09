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

#ifndef FSO_CHANNEL_H
#define FSO_CHANNEL_H

#include <vector>
#include <stdint.h>
#include "ns3/packet.h"
#include "ns3/channel.h"
#include "fso-signal-parameters.h"
#include "fso-propagation-loss-model.h"
#include "fso-phy.h"
#include "ns3/nstime.h"

namespace ns3 {

class NetDevice;
class PropagationDelayModel;

/**
 * \brief A Free space optical channel
 * \ingroup fso
 *
 * This fso channel implements the propagation model described in...
 * 
 *
 * This class is expected to be used in tandem with the ns3::FsoPhy
 * class and contains a list of ns3::FsoPropagationLossModel and a single ns3::PropagationDelayModel.
 * By default, no propagation models are set so, it is the caller's responsability
 * to set them before using the channel.
 */
class FsoChannel : public Channel
{
public:
  static TypeId GetTypeId (void);

  FsoChannel ();
  virtual ~FsoChannel ();

  //inherited from Channel.
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

  /**
   * Adds the given FsoPhy to the PHY list
   *
   * \param phy the FsoPhy to be added to the PHY list
   */
  void Add (Ptr<FsoPhy> phy);

  /**
   * \param loss the new propagation loss model.
   */
  void AddFsoPropagationLossModel (Ptr<FsoPropagationLossModel> loss);
  /**
   * \param delay the new propagation delay model.
   */
  void SetPropagationDelayModel (Ptr<PropagationDelayModel> delay);

  /**
   * Sends a packet to all the FsoPhys attached to the channel and schedules the
   * receive. 
   *
   * \param sender the device from which the packet is originating.
   * \param packet the packet to send
   * \param fsoSignalParams the struct containing all the signal parameters
   * \param duration the transmission duration associated to the packet
   */
  void Send (Ptr<FsoPhy> sender, Ptr<const Packet> packet,
             Ptr<FsoSignalParameters> fsoSignalParams, Time duration);

  /**
   * Assign a fixed random variable stream number to the random variables
   * used by this model.  Return the number of streams (possibly zero) that
   * have been assigned.
   *
   * \param stream first stream index to use
   *
   * \return the number of stream indices assigned by this model
   */
  int64_t AssignStreams (int64_t stream);

  //MDP - is this needed? came from copy of FsoChannel
  /**
   * TracedCallback signature for path loss calculation events.
   *
   * \param [in] txPhy The TX FsoPhy instance.
   * \param [in] rxPhy The RX FsoPhy instance.
   * \param [in] lossDb The loss value, in dB.
   */
  typedef void (* LossTracedCallback)
    (Ptr<FsoPhy> txPhy, Ptr<FsoPhy> rxPhy,
     double lossDb);


private:
  /**
   * A vector of pointers to YansWifiPhy.
   */
  typedef std::vector<Ptr<FsoPhy> > PhyList; 
  typedef std::vector<Ptr<FsoPropagationLossModel> > LossList;

  PhyList m_phyList;                   //!< List of FsoPhys connected to this FsoChannel
  LossList m_lossList;    //!< List of propagation loss models
  Ptr<PropagationDelayModel> m_delay;  //!< Propagation delay model
};



} //namespace ns3

#endif /* FSO_CHANNEL_H */
