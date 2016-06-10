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

#include "optical-phy.h"
#include "optical-fiber-channel.h"
#include "optical-net-device.h"
#include "optical-fiber-unidirectional-channel.h"
#include "optical-spectrum-signal-parameters.h"

#include <ns3/isotropic-antenna-model.h>

#include <ns3/log.h>
#include <ns3/enum.h>
#include <ns3/packet.h>
#include <ns3/packet-burst.h>
#include <ns3/node.h>
#include <ns3/mobility-model.h>
#include <ns3/simulator.h>

#include <limits>

NS_LOG_COMPONENT_DEFINE ("OpticalPhy");

namespace ns3 {

class Packet;
class Object;

NS_OBJECT_ENSURE_REGISTERED (OpticalPhy);

TypeId
OpticalPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::OpticalPhy")
          .SetParent<SpectrumPhy> ()
          .AddConstructor<OpticalPhy> ()
          .AddAttribute ("OpticalSpectrumModel",
                         "Channel spacing",
                         EnumValue (OpticalSpectrumValueHelper::OPTICAL_SPECTRUM_MODEL_100GHZ_CHANNEL_SPACING),
                         MakeEnumAccessor (&OpticalPhy::SetSpectrumModel),
                         MakeEnumChecker (OpticalSpectrumValueHelper::OPTICAL_SPECTRUM_MODEL_12500MHZ_CHANNEL_SPACING, "Optical Spectrum Model 12500MHz Channel Spacing",
                                          OpticalSpectrumValueHelper::OPTICAL_SPECTRUM_MODEL_25GHZ_CHANNEL_SPACING, "Optical Spectrum Model 25GHz Channel Spacing",
                                          OpticalSpectrumValueHelper::OPTICAL_SPECTRUM_MODEL_50GHZ_CHANNEL_SPACING, "Optical Spectrum Model 50GHz Channel Spacing",
                                          OpticalSpectrumValueHelper::OPTICAL_SPECTRUM_MODEL_100GHZ_CHANNEL_SPACING, "Optical Spectrum Model 100GHz Channel Spacing",
                                          OpticalSpectrumValueHelper::OPTICAL_SPECTRUM_MODEL_200GHZ_CHANNEL_SPACING, "Optical Spectrum Model 200GHz Channel Spacing"))
          .AddTraceSource ("StartTrace",
                           "Tracing change of object state to started",
                           MakeTraceSourceAccessor (&OpticalPhy::m_startTrace))
          .AddTraceSource ("StopTrace",
                           "Tracing change of object state to stop",
                           MakeTraceSourceAccessor (&OpticalPhy::m_stopTrace))
          ;
  return tid;
}

OpticalPhy::OpticalPhy ()
  :
    m_antennaModel (CreateObject<IsotropicAntennaModel> ()),
    m_antennaAngles (Angles ())
{
  NS_LOG_FUNCTION (this);
}

OpticalPhy::~OpticalPhy ()
{
  NS_LOG_FUNCTION (this);
}

Ptr<NetDevice>
OpticalPhy::GetDevice (void)
{
  NS_LOG_FUNCTION (this);
  return m_device;
}

void
OpticalPhy::SetDevice (Ptr<NetDevice> netDevice)
{
  NS_LOG_FUNCTION (this << netDevice);
  
  Ptr<OpticalNetDevice> wdmNetDevice = netDevice->GetObject<OpticalNetDevice> ();
  NS_ASSERT (wdmNetDevice != 0);
  
  m_device = wdmNetDevice;
}

Ptr<MobilityModel>
OpticalPhy::GetMobility ()
{
  NS_LOG_FUNCTION (this);
  return m_device->GetNode ()->GetObject<MobilityModel> ();
}

void
OpticalPhy::SetMobility (Ptr<MobilityModel> mobility)
{
  NS_LOG_FUNCTION (this << mobility);
  m_device->GetNode ()->AggregateObject (mobility);
}

Ptr<SpectrumChannel>
OpticalPhy::GetChannel (void) const
{
  NS_LOG_FUNCTION (this);
  return m_channel;
}

void
OpticalPhy::SetChannel (Ptr<SpectrumChannel> ch)
{
  NS_LOG_FUNCTION (this);
  
  Ptr<OpticalFiberChannel> wdmChannel = ch->GetObject<OpticalFiberChannel> ();
  NS_ASSERT (wdmChannel != 0);
  
  m_channel = wdmChannel;
}

Ptr<const SpectrumModel>
OpticalPhy::GetRxSpectrumModel () const
{
  NS_LOG_FUNCTION (this);
  return m_spectrumModel;
}

Ptr<AntennaModel>
OpticalPhy::GetRxAntenna ()
{
  NS_LOG_FUNCTION (this);
  return m_antennaModel;
}

void
OpticalPhy::StartTx (Ptr<SpectrumSignalParameters> params)
{
  NS_LOG_FUNCTION (this);
  
  NS_ASSERT_MSG (m_channel != 0, "Phy isn't attached to SpectrumChannel");

  (*params->psd) += m_antennaModel->GetGainDb (m_antennaAngles); // FIXME cleanup

  uint32_t channelNumber = GetSpectrumValueFrequencyChannel (params->psd);
  SetTxChannelBusyDuringTx (channelNumber, params->duration);
  m_channel->StartTx (params);
}

void
OpticalPhy::StartRx (Ptr<SpectrumSignalParameters> params)
{
  NS_LOG_FUNCTION (this);
  
  NS_ASSERT_MSG (m_device != 0, "Phy isn't attached to NetDevice");
  
  m_device->Receive (params, this);
}

void
OpticalPhy::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
  Start ();
  SpectrumPhy::DoInitialize ();
}

void
OpticalPhy::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  m_channel = 0;
  m_device = 0;
  m_antennaModel = 0;
  m_spectrumModel = 0;
  SpectrumPhy::DoDispose ();
}

void
OpticalPhy::SetSpectrumModel (OpticalSpectrumValueHelper::OpticalSpectrumModelsEnum model)
{
  NS_LOG_FUNCTION (this << model);
  switch (model)
    {
    case OpticalSpectrumValueHelper::OPTICAL_SPECTRUM_MODEL_12500MHZ_CHANNEL_SPACING:
      m_spectrumModel = OpticalDwdmSpectrumModel12500MhzChannelSpacing;
      break;
    case OpticalSpectrumValueHelper::OPTICAL_SPECTRUM_MODEL_25GHZ_CHANNEL_SPACING:
      m_spectrumModel = OpticalDwdmSpectrumModel25GhzChannelSpacing;
      break;
    case OpticalSpectrumValueHelper::OPTICAL_SPECTRUM_MODEL_50GHZ_CHANNEL_SPACING:
      m_spectrumModel = OpticalDwdmSpectrumModel50GhzChannelSpacing;
      break;
    case OpticalSpectrumValueHelper::OPTICAL_SPECTRUM_MODEL_100GHZ_CHANNEL_SPACING:
      m_spectrumModel = OpticalDwdmSpectrumModel100GhzChannelSpacing;
      break;
    case OpticalSpectrumValueHelper::OPTICAL_SPECTRUM_MODEL_200GHZ_CHANNEL_SPACING:
      m_spectrumModel = OpticalDwdmSpectrumModel200GhzChannelSpacing;
      break;
    default:
      NS_FATAL_ERROR ("Unsupported OpticalSpectrumModel in OpticalPhy::SetSpectrumModel ()");
    }
}

uint32_t
OpticalPhy::GetSpectrumValueFrequencyChannel (Ptr<SpectrumValue> psd)
{
  NS_LOG_FUNCTION (this << psd);

  uint32_t channel = psd->GetSpectrumModel ()->GetNumBands (); // FIXME zašto ne dohvaćam spectrum model od Phya samog
  double power = -std::numeric_limits<double>::infinity ();
  for (uint32_t i = 0; i < psd->GetSpectrumModel ()->GetNumBands (); i++)
    {
      if (power > (*psd)[i])
        {
          channel = i;
        }
    }
  
  return channel;
}

void
OpticalPhy::SetTxChannelBusyDuringTx (uint32_t channelNumber, Time duration)
{
  NS_LOG_FUNCTION (this << channelNumber << duration);

  SetTxChannelStatus (channelNumber, true);
  Simulator::Schedule (duration, &OpticalPhy::SetTxChannelStatus, this, channelNumber, false);
}

bool
OpticalPhy::GetTxChannelStatus (uint32_t channelNumber)
{
  return m_txChannelBusy[channelNumber];
}

void
OpticalPhy::SetTxChannelStatus (uint32_t channelNumber, bool status)
{
  NS_LOG_FUNCTION (this << channelNumber << status);

  m_txChannelBusy[channelNumber] = status;
}

} // namespace ns3

