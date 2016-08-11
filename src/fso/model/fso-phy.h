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
#include "ns3/timer.h"
#include <ns3/packet.h>
#include <ns3/net-device.h>

#include "fso-error-model.h"
#include "fso-signal-parameters.h"
#include "fso-channel.h"
#include "optical-rx-antenna-model.h"
#include "laser-antenna-model.h"

namespace ns3 {

class FsoChannel;
class MobilityModel;
class FsoErrorModel;
class NetDevice;

/**
 * \ingroup fso
 *
 *  Base class for free-space optical PHY layer
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

  enum State
  {
    /**
     * The PHY layer is IDLE.
     */
    IDLE,
    /**
     * The PHY layer is sending a packet.
     */
    TX
  };

  FsoPhy::State GetTxState ();

  /**
   * arg1: packet received successfully
   * arg2: snr of packet 
   * arg3: signal parameters of packet
   */
  typedef Callback<void, Ptr<Packet>, double, Ptr<FsoSignalParameters> > RxOkCallback;
  /**
   * arg1: packet received unsuccessfully
   * arg2: snr of packet
   */
  typedef Callback<void, Ptr<Packet>, double> RxErrorCallback;

  /**
   * Switch from RX after the reception was successful.
   *
   * \param packet the successfully received packet
   * \param snr the SNR of the received packet
   * \param params signal parameters of the packet
   */
  virtual void SwitchFromRxEndOk (Ptr<Packet> packet, double snr, Ptr<FsoSignalParameters> params);

  /**
   * Switch from RX after the reception failed.
   *
   * \param packet the packet that we failed to received
   * \param snr the SNR of the received packet
   */
  virtual void SwitchFromRxEndError (Ptr<Packet> packet, double snr);

  /**
   * \param callback the callback to invoke
   *        upon successful packet reception.
   */
  virtual void SetReceiveOkCallback (RxOkCallback callback);
  /**
   * \param callback the callback to invoke
   *        upon erroneous packet reception.
   */
  virtual void SetReceiveErrorCallback (RxErrorCallback callback);

  /**
   * Set the associated NetDevice instance
   *
   * \param d the NetDevice instance
   */
  virtual void SetDevice (Ptr<NetDevice> d);

  /**
   * Get the associated NetDevice instance
   *
   * \return a Ptr to the associated NetDevice instance
   */
  virtual Ptr<NetDevice> GetDevice () const;

  /**
   * Set the mobility model associated with this device.
   *
   * \param m the mobility model
   */
  virtual void SetMobility (Ptr<MobilityModel> m);

  /**
   * Get the associated MobilityModel instance
   *
   * \return a Ptr to the associated MobilityModel instance
   */
  virtual Ptr<MobilityModel> GetMobility () const;

  /**
   * Set the channel attached to this device.
   *
   * \param c pointer to the channel
   */
  virtual void SetChannel (Ptr<FsoChannel> c);

  /**
   * Return the FsoChannel this FsoPhy is connected to.
   *
   * \return the FsoChannel this FsoPhy is connected to
   */
  virtual Ptr<FsoChannel> GetChannel () const;

  /**
   * Get the AntennaModel used by the NetDevice for reception
   *
   * \return a Ptr to the AntennaModel used by the NetDevice for reception
   */
  virtual Ptr<OpticalRxAntennaModel> GetRxAntenna () const;

  /**
   * Get the AntennaModel used by the NetDevice for transmission
   *
   * \return a Ptr to the AntennaModel used by the NetDevice for transmission
   */
  virtual Ptr<LaserAntennaModel> GetTxAntenna () const;

  /**
   * \param packet the packet to send
   * \param fsoSignalParams pointer to the optical signal parameters
   */
  virtual void Transmit (Ptr<const Packet> packet, Ptr<FsoSignalParameters> fsoSignalParams);


  /**
   * Starting receiving the payload of a packet (i.e. the first bit of the packet has arrived).
   *
   * \param packet the arriving packet
   * \param fsoSignalParams pointer to the optical signal parameters
   */
  virtual void Receive (Ptr<Packet> packet, Ptr<FsoSignalParameters> fsoSignalParams);


  /**
   * Calculate how long the transmit time will be (size * symbol period)
   *
   * \param size the size of the packet in bits
   * \param fsoSignalParams pointer to the optical signal parameters
   */
  virtual Time CalculateTxDuration (uint32_t size, Ptr<FsoSignalParameters> fsoSignalParams);

  /**
   * Assign the receiver and transmitter antennas to this Phy
   *
   * \param txAnntenna pointer to the transmitter antenna
   * \param txAnntenna pointer to the receiver antenna
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
   * \return the bit rate
   */
  virtual double GetBitRate () const;


protected:
  //Inherited from Object
  virtual void DoDispose ();

private:
  Ptr<FsoChannel>               m_channel;        //!< FsoChannel that this FsoPhy is connected to
  Ptr<NetDevice>                m_device;         //!< Pointer to the device
  Ptr<MobilityModel>            m_mobility;       //!< Pointer to the mobility model
  Ptr<LaserAntennaModel>        m_txAntenna;      //!< Pointer to the TX antenna model
  Ptr<OpticalRxAntennaModel>    m_rxAntenna;      //!< Pointer to the RX antenna model
  Ptr<FsoErrorModel>            m_errorModel;     //!< Pointer to the error model
  
  State                         m_txState;        //!< transmit state of the Phy
  Timer                         m_txDurationTimer;//!< Timer for transmit transmit duration

  RxOkCallback                  m_rxOkCallback;   //!< Callback for received packet
  RxErrorCallback               m_rxErrorCallback;//!< Callback for received packet with packet error
  
  double                        m_bitRate;        //!< bit rate associated with the Phy

  /**
   * Set Phy state to TX and schedule switch back to IDLE
   *
   * \param duration the transmit duration
   */
  virtual void SwitchToTx (double duration);

  /**
   * Set Phy state to IDLE
   *
   */  
  virtual void SwitchToIdle ();

};





} // namespace ns3


#endif /* FSO_PHY_H */
