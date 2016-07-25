/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "fso-error-model.h"
#include <ns3/double.h>
#include <ns3/log.h>
#include <cmath>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FsoDownLinkErrorModel");

NS_OBJECT_ENSURE_REGISTERED (FsoErrorModel);
NS_OBJECT_ENSURE_REGISTERED (FsoDownLinkErrorModel);



TypeId FsoErrorModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoErrorModel")
    .SetParent<Object> ()
    .SetGroupName ("Fso")
  ;
  return tid;
}

TypeId 
FsoDownLinkErrorModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoDownLinkErrorModel")
    .SetParent<FsoErrorModel> ()
    .SetGroupName ("Fso")
    .AddConstructor<FsoDownLinkErrorModel> ()
  ;
  return tid;
}

FsoDownLinkErrorModel::FsoDownLinkErrorModel ()
{
  NS_LOG_FUNCTION (this);
}

FsoDownLinkErrorModel::~FsoDownLinkErrorModel ()
{
  NS_LOG_FUNCTION (this);
}

double
FsoDownLinkErrorModel::CalculateMeanSnr () const
{
  return 0.0;
}

double
FsoDownLinkErrorModel::CalculateMeanBer () const
{
  return 0.0;
}

double 
FsoDownLinkErrorModel::GetChunkSuccessRate (FsoSignalParameters fsoSignalParams, uint32_t nbits)
{
  NS_LOG_DEBUG ("ErrorModel: packet size=" << nbits << " bits, scintIndex=" << fsoSignalParams.scintillationIndex << ", meanIrradiance=" << fsoSignalParams.meanIrradiance); 
  return CalculateRxIrradiance(fsoSignalParams.scintillationIndex, fsoSignalParams.meanIrradiance);
}

double 
FsoDownLinkErrorModel::CalculateRxIrradiance (double scintillationIndex, double meanIrradiance)
{
  double mu = -0.5*scintillationIndex;
  double sigma = std::sqrt(scintillationIndex);
  Ptr<LogNormalRandomVariable> x = CreateObject<LogNormalRandomVariable> ();
  x->SetAttribute ("Mu", DoubleValue (mu));
  x->SetAttribute ("Sigma", DoubleValue (sigma));

  double normalizedIrradiance = x->GetValue ();
  //double normalizedIrradiance = m_logNormalDist.GetValue(-0.5*scintillationIndex, std::sqrt(scintillationIndex));
  
  NS_LOG_DEBUG ("ErrorModel: Normalized Irradiance=" << normalizedIrradiance);  

  return normalizedIrradiance*meanIrradiance;
}

} //namespace ns3
