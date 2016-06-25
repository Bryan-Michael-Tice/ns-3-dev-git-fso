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


#include "fso-propagation-loss-model.h"
#include <ns3/math.h>
#include <ns3/log.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FsoPropagationLossModel");

NS_OBJECT_ENSURE_REGISTERED (FsoPropagationLossModel);

FsoPropagationLossModel::FsoPropagationLossModel ()
{
}

FsoPropagationLossModel::~FsoPropagationLossModel ()
{
}

void
FsoPropagationLossModel::DoDispose ()
{
  
}

TypeId
FsoPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoPropagationLossModel")
    .SetParent<Object> ()
    .SetGroupName ("Fso")
  ;
  return tid;
}

void 
FsoPropagationLossModel::CalculateScintillationIdx (double f, double hGS, double hSat, double e) const
{
  double k = (2*PI)/(DoubleValue(3e8)/f);//Wave number - 2*pi/wavelength
   
  m_scintIdx = 2.25*pow(k,7.0/6.0)*(1/pow(cos(e),11.0/6.0));//*integral();//MDP - get solved expression of the integral
}

void 
FsoPropagationLossModel::CalculateMeanIrradiance (double txBeamRadius)
{
  double effectiveSpotSize = m_rxDiffractiveBeamRadius;//This is an approximation which is only valid for downlink, high elevation path
  double r = 0.0;//This is the distance in the xy plane from the optical axis (propagation along z-axis)
  m_meanIrradiance = (pow(txBeamRadius, 2.0)/pow(effectiveSpotSize, 2.0))*exp((-2*r)/(pow(effectiveSpotSize, 2.0)));
}

void
FsoPropagationLossModel::CalculateDiffractiveBeamRadius (double f, double d, double txBeamRadius, double txPhaseFrontRadius)
{
  double theta0 = 1 - (d/phaseFrontRadius);
  double waveLength = (DoubleValue (3e8))/f;//wavelength = speed of light/frequency
  double lambda0 = (2*d)/((2*PI/waveLength)*pow(txBeamRadius, 2.0));

  m_rxDiffractiveBeamRadius = txBeamRadius*sqrt(pow(theta0, 2.0) + pow(lambda0, 2.0));
}



} // namespace ns3
