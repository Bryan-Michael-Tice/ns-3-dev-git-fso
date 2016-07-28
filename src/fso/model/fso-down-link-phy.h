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
 */


#ifndef FSO_DOWN_LINK_PHY_H
#define FSO_DOWN_LINK_PHY_H


#include <ns3/mobility-model.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/net-device.h>
#include <ns3/data-rate.h>
#include <ns3/event-id.h>

#include "fso-phy.h"
#include "fso-error-model.h"
#include "fso-signal-parameters.h"
#include "fso-channel.h"
#include "optical-rx-antenna-model.h"
#include "laser-antenna-model.h"

namespace ns3 {


class FsoDownLinkPhy : public FsoPhy
{
public:

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();

  enum State
  {
    /**
     * The PHY layer is IDLE.
     */
    IDLE,
    /**
     * The PHY layer is sending a packet.
     */
    TX,
    /**
     * The PHY layer is receiving a packet.
     */
    RX
  };

  FsoDownLinkPhy ();
  virtual ~FsoDownLinkPhy ();

  // inherited from FsoPhy
  virtual void SetDevice (Ptr<NetDevice> d);
  virtual Ptr<NetDevice> GetDevice () const;
  virtual void SetMobility (Ptr<MobilityModel> m);
  virtual Ptr<MobilityModel> GetMobility () const;
  virtual void SetChannel (Ptr<FsoChannel> c);
  virtual Ptr<FsoChannel> GetChannel () const;
  virtual Ptr<OpticalRxAntennaModel> GetRxAntenna () const;
  virtual Ptr<LaserAntennaModel> GetTxAntenna () const;
  virtual void SendPacket (Ptr<const Packet> packet, FsoSignalParameters fsoSignalParams);
  virtual void ReceivePacket (Ptr<Packet> packet, FsoSignalParameters fsoSignalParams);
  virtual Time CalculateTxDuration (uint32_t size, FsoSignalParameters fsoSignalParams);
  
  /**
   * Assign the receiver and transmitter antennas to this Phy
   *
   * \param txAnntenna the transmitter antenna
   * \param txAnntenna the receiver antenna
   */
  virtual void SetAntennas (Ptr<LaserAntennaModel> txAntenna, Ptr<OpticalRxAntennaModel> rxAntenna);

  /**
   * Assign the error model to be used by this Phy
   *
   * \param errModel pointer the error model
   */
  virtual void SetErrorModel (Ptr<FsoErrorModel> errModel);


  /**
   * Assign the bit rate to be used by this Phy
   *
   * \param bitRate the bit rate
   */
  virtual void SetBitRate (double bitRate);


  /**
   * Return the bit rate used by this Phy
   *
   * @return the bit rate
   */
  virtual double GetBitRate () const;


  

private:
  Ptr<FsoChannel>               m_channel;        //!< FsoChannel that this FsoPhy is connected to
  Ptr<NetDevice>                m_device;         //!< Pointer to the device
  Ptr<MobilityModel>            m_mobility;       //!< Pointer to the mobility model
  Ptr<LaserAntennaModel>        m_txAntenna;      //!< Pointer to the TX antenna model
  Ptr<OpticalRxAntennaModel>    m_rxAntenna;      //!< Pointer to the RX antenna model
  Ptr<FsoErrorModel>            m_errorModel;     //!< Pointer to the error model
  State                         m_state;          //!< state of the Phy
  
  double                        m_bitRate;        //!< bit rate associated to the Phy

protected:
  //Inherited from Object
  virtual void DoDispose ();

};

}


#endif /* FSO_DOWN_LINK_PHY_H */
