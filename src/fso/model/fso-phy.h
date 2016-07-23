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

#ifndef FSO_PHY_H
#define FSO_PHY_H


#include <ns3/object.h>
#include <ns3/nstime.h>

namespace ns3 {

class FsoChannel;
class AntennaModel;
class MobilityModel;
class NetDevice;
struct FsoSignalParameters;

/**
 * \ingroup fso
 *
 * Abstract base class for free-space optical PHY layers
 *
 */
class FsoPhy  : public Object
{

public:
  
  FsoPhy ();
  virtual ~FsoPhy ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * Set the associated NetDevice instance
   *
   * @param d the NetDevice instance
   */
  virtual void SetDevice (Ptr<NetDevice> d) = 0;

  /**
   * Get the associated NetDevice instance
   *
   * @return a Ptr to the associated NetDevice instance
   */
  virtual Ptr<NetDevice> GetDevice () const = 0;

  /**
   * Set the mobility model associated with this device.
   *
   * @param m the mobility model
   */
  virtual void SetMobility (Ptr<MobilityModel> m) = 0;

  /**
   * Get the associated MobilityModel instance
   *
   * @return a Ptr to the associated MobilityModel instance
   */
  virtual Ptr<MobilityModel> GetMobility () = 0;

  /**
   * Set the channel attached to this device.
   *
   * @param c the channel
   */
  virtual void SetChannel (Ptr<FsoChannel> c) = 0;

  /**
   * Return the WifiChannel this WifiPhy is connected to.
   *
   * \return the WifiChannel this WifiPhy is connected to
   */
  virtual Ptr<FsoChannel> GetChannel () const = 0;

  /**
   * Get the AntennaModel used by the NetDevice for reception
   *
   * @return a Ptr to the AntennaModel used by the NetDevice for reception
   */
  virtual Ptr<AntennaModel> GetRxAntenna () = 0;


  /**
   * \param packet the packet to send
   * \param FsoSignalParameters contains the signal parameters
   */
  virtual void SendPacket (Ptr<const Packet> packet, FsoSignalParameters fsoSignalParams) = 0;


  /**
   * Starting receiving the payload of a packet (i.e. the first bit of the packet has arrived).
   *
   * \param packet the arriving packet
   * \param FsoSignalParameters contains the signal parameters
   */
  virtual void ReceivePacket (Ptr<Packet> packet, FsoSignalParameters fsoSignalParams) = 0;


  /**

   */
  virtual Time CalculateTxDuration (uint32_t size, FsoSignalParameters fsoSignalParams) = 0;


private:
  /**
   * \brief Copy constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  FsoPhy (FsoPhy const &);
  /**
   * \brief Copy constructor
   *
   * Defined and unimplemented to avoid misuse
   * \returns
   */
  FsoPhy& operator= (FsoPhy const &);
};





} // namespace ns3


#endif /* FSO_PHY_H */
