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


#include "fso-mean-irradiance-model.h"
#include <ns3/math.h>
#include <ns3/log.h>
#include "ns3/double.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FsoMeanIrradianceModel");

NS_OBJECT_ENSURE_REGISTERED (FsoMeanIrradianceModel);

FsoMeanIrradianceModel::FsoMeanIrradianceModel ()
{
}

FsoMeanIrradianceModel::~FsoMeanIrradianceModel ()
{
}

void
FsoMeanIrradianceModel::DoDispose ()
{
  
}

TypeId
FsoMeanIrradianceModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoMeanIrradianceModel")
    .SetParent<Object> ()
    .SetGroupName ("Fso")
    .AddConstructor<FsoMeanIrradianceModel> ();

  return tid;
}

void
FsoMeanIrradianceModel::UpdateSignalParams(FsoSignalParameters& fsoSignalParams, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b)
{
  double distance = a->GetDistanceFrom (b);

  NS_LOG_DEBUG ("MeanIrradiance: distance=" << distance << "m, frequency=" << fsoSignalParams.frequency << "Hz, beamwidth=" << fsoSignalParams.txBeamwidth << "m, phase front radius=" << fsoSignalParams.txPhaseFrontRadius); 

  double rxDiffractiveBeamRadius = CalculateDiffractiveBeamRadius(distance, fsoSignalParams.frequency, fsoSignalParams.txBeamwidth, fsoSignalParams.txPhaseFrontRadius);

fsoSignalParams.meanIrradiance = CalculateMeanIrradiance(fsoSignalParams.txBeamwidth, rxDiffractiveBeamRadius);

NS_LOG_DEBUG ("MeanIrradiance: result=" << fsoSignalParams.meanIrradiance);
}

double 
FsoMeanIrradianceModel::CalculateMeanIrradiance (double txBeamRadius, double rxDiffractiveBeamRadius)
{
  double effectiveSpotSize = rxDiffractiveBeamRadius;//This is an approximation which is only valid for downlink, high elevation path
  double r = 0.0;//This is the distance in the xy plane from the optical axis (propagation along z-axis)
  return ((pow(txBeamRadius, 2.0)/pow(effectiveSpotSize, 2.0))*exp((-2*r)/(pow(effectiveSpotSize, 2.0))));
}

double
FsoMeanIrradianceModel::CalculateDiffractiveBeamRadius (double d, double f, double txBeamRadius, double txPhaseFrontRadius)
{
  double theta0 = 1 - (d/txPhaseFrontRadius);
  double waveLength = (3e8)/f;//wavelength = speed of light/frequency
  double lambda0 = (2*d)/((2*M_PI/waveLength)*pow(txBeamRadius, 2.0));

  NS_LOG_DEBUG ("MeanIrradiance: theta0=" << theta0);
  NS_LOG_DEBUG ("MeanIrradiance: lambda0=" << lambda0);
  NS_LOG_DEBUG ("MeanIrradiance: W=" << txBeamRadius*sqrt(pow(theta0, 2.0) + pow(lambda0, 2.0)));    

  return (txBeamRadius*sqrt(pow(theta0, 2.0) + pow(lambda0, 2.0)));
}



} // namespace ns3
