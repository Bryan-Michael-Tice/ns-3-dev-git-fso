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


#include "fso-free-space-loss-model.h"
#include <ns3/math.h>
#include <ns3/log.h>
#include "ns3/double.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FsoFreeSpaceLossModel");

NS_OBJECT_ENSURE_REGISTERED (FsoFreeSpaceLossModel);

FsoFreeSpaceLossModel::FsoFreeSpaceLossModel ()
{
}

FsoFreeSpaceLossModel::~FsoFreeSpaceLossModel ()
{
}

void
FsoFreeSpaceLossModel::DoDispose ()
{
  
}

TypeId
FsoFreeSpaceLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoFreeSpaceLossModel")
    .SetParent<Object> ()
    .SetGroupName ("Fso")
    .AddConstructor<FsoFreeSpaceLossModel> ();

  return tid;
}

int64_t 
FsoFreeSpaceLossModel::DoAssignStreams (int64_t stream)
{
  return 0.0;
}

void
FsoFreeSpaceLossModel::DoUpdateSignalParams(Ptr<FsoSignalParameters> fsoSignalParams, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b)
{
  double distance = a->GetDistanceFrom (b);
  fsoSignalParams->pathLoss = CalculateFreeSpaceLoss(distance, fsoSignalParams->wavelength);
  fsoSignalParams->power -= fsoSignalParams->pathLoss;
  NS_LOG_DEBUG ("FreeSpaceLoss: distance=" << distance << "m, frequency=" << fsoSignalParams->frequency << "Hz, loss=" << fsoSignalParams->pathLoss << "dB");
}


double
FsoFreeSpaceLossModel::CalculateFreeSpaceLoss (double d, double wavelength)
{
  return (20.0*std::log10(4.0*M_PI*d/wavelength));//return free space path loss in dB
}



} // namespace ns3
