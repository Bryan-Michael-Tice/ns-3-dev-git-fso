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


#include "fso-down-link-scintillation-index-model.h"
#include "ns3/double.h"
#include <ns3/math.h>
#include <ns3/log.h>
#include <utility>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FsoDownLinkScintillationIndexModel");

NS_OBJECT_ENSURE_REGISTERED (FsoDownLinkScintillationIndexModel);

FsoDownLinkScintillationIndexModel::FsoDownLinkScintillationIndexModel ()
{
}

FsoDownLinkScintillationIndexModel::~FsoDownLinkScintillationIndexModel ()
{
}

void
FsoDownLinkScintillationIndexModel::DoDispose ()
{
  
}

TypeId
FsoDownLinkScintillationIndexModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoDownLinkScintillationIndexModel")
    .SetParent<Object> ()
    .SetGroupName ("Fso")
    .AddConstructor<FsoDownLinkScintillationIndexModel> ()
    /*.AddAttribute ("WindSpeed",
                   "The rms windspeed (meters/second)",
                   DoubleValue (21),
                   MakeDoubleAccessor (&FsoDownLinkScintillationIndexModel::SetRmsWindSpeed,
                                       &FsoDownLinkScintillationIndexModel::GetRmsWindSpeed),
                   MakeDoubleChecker<double> (0, 250))

    .AddAttribute ("GroundRefractiveIndex",
                   "The nominal value of Hufnagel-Valley index of refraction (m^-2/3) on the ground",
                   DoubleValue (1.7e-14),
                   MakeDoubleAccessor (&FsoDownLinkScintillationIndexModel::SetGndRefractiveIdx,
                                       &FsoDownLinkScintillationIndexModel::GetGndRefractiveIdx),
                   MakeDoubleChecker<double> (0, 1))*/
  ;
  return tid;
}

void 
FsoDownLinkScintillationIndexModel::UpdateSignalParams (FsoSignalParameters& fsoSignalParams, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b)
{
  NS_LOG_FUNCTION (this);

  double heightTx = 0.0;
  double heightRx = 0.0;
  double elevation = 30.0*(M_PI/180.0);//MDP - This is fixed for now, Satellite mobility model should provide an elevation
 
  //Determine which mobility model is the tx,rx for downlink
  NS_ASSERT (a->GetPosition().z >= a->GetPosition().z);

  fsoSignalParams.scintillationIndex = CalculateScintillationIdx (fsoSignalParams.frequency, heightTx, heightRx, elevation);

}

double
FsoDownLinkScintillationIndexModel::CalculateScintillationIdx (double f, double hTx, double hRx, double e) const
{
  NS_LOG_FUNCTION (this);

  double k = (2*M_PI)/((3e8)/f);//Wave number - 2*pi/wavelength
   
  return (2.25*pow(k,7.0/6.0)*(1/pow(cos(e),11.0/6.0)));//*integral();//MDP - need numerical solver or look-up table for integral
}



} // namespace ns3
