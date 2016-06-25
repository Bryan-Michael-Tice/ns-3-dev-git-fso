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


#ifndef FSO_CHANNEL_H
#define FSO_CHANNEL_H

#include <ns3/object.h>
#include <ns3/nstime.h>
#include <ns3/channel.h>
#include <ns3/fso-signal-parameters.h>

namespace ns3 {


class PacketBurst;
class FsoPhy;
class FsoPropagationLossModel;
class PropagationDelayModel;

/**
 * \ingroup fso
 *
 * Defines the interface for free-space optical channel implementations
 *
 */
class FsoChannel : public Channel
{
public:
  virtual ~FsoChannel ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * Set the frequency-dependent propagation loss model to be used
   * \param loss a pointer to the propagation loss model to be used.
   */
  virtual void AddFsoPropagationLossModel (Ptr<FsoPropagationLossModel> loss) = 0;

  /**
   * Set the  propagation delay model to be used
   * \param delay Ptr to the propagation delay model to be used.
   */
  virtual void SetPropagationDelayModel (Ptr<PropagationDelayModel> delay) = 0;


  /**
   * Used by attached PHY instances to transmit signals on the channel
   *
   * @param params the parameters of the signals being transmitted
   */
  virtual void StartTx (Ptr<FsoSignalParameters> params) = 0;

  /**
   * @brief Add a FsoPhy to a channel, so it can receive packets
   *
   * This method is used to attach a FsoPhy instance to a
   * FsoChannel instance, so that the FsoPhy can receive
   * packets sent on that channel. Note that a FsoPhy that only
   * transmits (without receiveing ever) does not need to be added to
   * the channel.
   *
   * This method is to be implemented by all classes inheriting from
   * Fsohannel.
   *
   * @param phy the FsoPhy instance to be added to the channel as
   * a receiver.
   */
  virtual void AddRx (Ptr<FsoPhy> phy) = 0;

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
  
};



}



#endif /* FSO_CHANNEL_H */
