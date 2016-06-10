/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012,2014 University of Zagreb
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
 * Author: Vedran Miletić <rivanvx@gmail.com>
 */

#ifndef WDM_PHY_H
#define WDM_PHY_H

#include "optical-fiber-channel.h"
#include "optical-spectrum-value-helper.h"

#include <ns3/spectrum-phy.h>
#include <ns3/angles.h>
#include <ns3/start-stop-functionality.h>

#include <map>
#include <string>

namespace ns3 {

class Packet;
class Channel;
class OpticalFiberChannel;
class NetDevice;
class OpticalNetDevice;
class SpectrumValue;

class OpticalPhy : public SpectrumPhy, public StartStopFunctionality
{
public:
  static TypeId GetTypeId (void);

  OpticalPhy ();
  virtual ~OpticalPhy ();

  virtual Ptr<NetDevice> GetDevice (void);
  virtual void SetDevice (Ptr<NetDevice> device);
  
  virtual void SetMobility (Ptr<MobilityModel> mobility);
  virtual Ptr<MobilityModel> GetMobility ();
  
  virtual Ptr<SpectrumChannel> GetChannel () const;
  virtual void SetChannel (Ptr<SpectrumChannel> ch);
  
  virtual Ptr<const SpectrumModel> GetRxSpectrumModel () const;
  virtual Ptr<AntennaModel> GetRxAntenna ();

  virtual void StartTx (Ptr<SpectrumSignalParameters> params);
  virtual void StartRx (Ptr<SpectrumSignalParameters> params);

protected:
  virtual void DoInitialize (void);
  virtual void DoDispose (void);
  
  void SetSpectrumModel (OpticalSpectrumValueHelper::OpticalSpectrumModelsEnum model);

  uint32_t GetSpectrumValueFrequencyChannel (Ptr<SpectrumValue> psd);

private:
  bool GetTxChannelStatus (uint32_t channelNumber);
  void SetTxChannelStatus (uint32_t channelNumber, bool status);

  bool GetRxChannelStatus (uint32_t channelNumber);
  void SetRxChannelStatus (uint32_t channelNumber, bool status);

  void SetTxChannelBusyDuringTx (uint32_t channelNumber, Time duration);
  void SetRxChannelBusyDuringRx (uint32_t channelNumber, Time duration);

  Ptr<OpticalNetDevice> m_device;
  Ptr<OpticalFiberChannel> m_channel;
  Ptr<AntennaModel> m_antennaModel;
  Angles m_antennaAngles;
  
  Ptr<SpectrumModel> m_spectrumModel;
  std::map<uint32_t, bool> m_rxChannelBusy;
  std::map<uint32_t, bool> m_txChannelBusy;
};

} // namespace ns3

#endif // WDM_PHY_H

