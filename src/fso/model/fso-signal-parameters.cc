/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 CTTC
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


#include <ns3/fso-signal-parameters.h>
#include <ns3/fso-phy.h>
#include <ns3/log.h>
#include <ns3/antenna-model.h>


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FsoSignalParameters");

FsoSignalParameters::FsoSignalParameters ()
{
  NS_LOG_FUNCTION (this);
}

FsoSignalParameters::~FsoSignalParameters ()
{
  NS_LOG_FUNCTION (this);
}

FsoSignalParameters::FsoSignalParameters (const FsoSignalParameters& p)
{
  NS_LOG_FUNCTION (this << &p);
  duration = p.duration;
  txPhy = p.txPhy;
  txAntenna = p.txAntenna;
  wavelength = p.wavelength;
  power = p.power;
  txBeamwidth = p.txBeamwidth;
  scintillationIndex = p.scintillationIndex;
  meanIrradiance = p.meanIrradiance;
  pathLoss = p.pathLoss;
}

Ptr<FsoSignalParameters>
FsoSignalParameters::Copy ()
{
  NS_LOG_FUNCTION (this);
  return Create<FsoSignalParameters> (*this);
}



} // namespace ns3
