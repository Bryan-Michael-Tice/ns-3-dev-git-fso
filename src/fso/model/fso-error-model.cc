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
#include <cmath>

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (FsoErrorModel);

TypeId FsoErrorModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoErrorModel")
    .SetParent<Object> ()
    .SetGroupName ("Fsos")
  ;
  return tid;
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
FsoDownLinkErrorModel::GetChunkSuccessRate (FsoSignalParameters fsoSignalParams, uint32_t nbits) const
{
  return 0.0;
}

double 
FsoDownLinkErrorModel::CalculateRxIrradiance (double scintillationIndex, double meanIrradiance)
{
  double normalizedIrradiance = m_logNormalDist.GetValue(-0.5*scintillationIndex, std::sqrt(scintillationIndex));

  return normalizedIrradiance*meanIrradiance;
}

} //namespace ns3
