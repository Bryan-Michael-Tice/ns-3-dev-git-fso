/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Michael Di Perna
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
 * Author: Michael Di Perna <diperna.michael@gmail.com>
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
    .AddAttribute ("WindSpeed",
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
                   MakeDoubleChecker<double> (0, 1))
  ;
  return tid;
}

int64_t 
FsoDownLinkScintillationIndexModel::DoAssignStreams (int64_t stream)
{
  return 0.0;
}

void 
FsoDownLinkScintillationIndexModel::DoUpdateSignalParams (Ptr<FsoSignalParameters> fsoSignalParams, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (a->GetPosition().z >= b->GetPosition().z);
  
  double heightTx = (a->GetPosition().z);
  double heightRx = (b->GetPosition().z);  

  double zenith = 30.0*(M_PI/180.0);//MDP - This is fixed for now, Satellite mobility model should provide an elevation or zenith angle

  fsoSignalParams->scintillationIndex = CalculateScintillationIdx (fsoSignalParams->frequency, heightTx, heightRx, zenith);

}

double
FsoDownLinkScintillationIndexModel::CalculateScintillationIdx (double f, double hTx, double hRx, double zenith) const
{
  NS_LOG_FUNCTION (this);

  double result = 0.0;
  double error = 0.0;

  HVFunctionParameters params;
  params.A = m_groundRefractiveIdx;
  params.v = m_rmsWindSpeed;
  params.hgs = hRx;

  gsl_integration_workspace * w = gsl_integration_workspace_alloc (10000);

  gsl_function F;
  F.function = &HVIntegralFunction;
  F.params = &params;

  //If the transmitter is higher than 20km, limit the integration to 20km
  //20km is the effective height of the turbulence (constant beyond 20km)
  if (hTx > 20000.0)
   {
     hTx = 20000.0;
   }
 
  gsl_integration_qags (&F, hRx, hTx, 1e-18, 1e-10, 10000, w, &result, &error);
  NS_LOG_DEBUG ("RESULT=" << result);
  NS_LOG_DEBUG ("ERROR=" << error);
  double wavelength = 3.0e8/f;
  double k = (2*M_PI)/wavelength;//Wave number - 2*pi/wavelength
   
  gsl_integration_workspace_free (w);
  return (2.25*std::pow(k, 7.0/6.0)*std::pow((1.0/cos(zenith)), 11.0/6.0))*result;
}

void 
FsoDownLinkScintillationIndexModel::SetRmsWindSpeed (double rmsWindSpeed)
{
  NS_LOG_FUNCTION (this);
  m_rmsWindSpeed = rmsWindSpeed;
}

double 
FsoDownLinkScintillationIndexModel::GetRmsWindSpeed () const
{
  return m_rmsWindSpeed;
}

void 
FsoDownLinkScintillationIndexModel::SetGndRefractiveIdx (double gndRefractiveIdx)
{
  NS_LOG_FUNCTION (this);
  m_groundRefractiveIdx = gndRefractiveIdx;
}

double 
FsoDownLinkScintillationIndexModel::GetGndRefractiveIdx () const
{
  return m_groundRefractiveIdx;
}

#ifdef HAVE_GSL
double
HVIntegralFunction (double h, void *params)
{
  double A = ((HVFunctionParameters *) params)->A;
  double v = ((HVFunctionParameters *) params)->v;
  double hgs = ((HVFunctionParameters *) params)->hgs;
  double IntegralFunction = (A*std::exp(-hgs/700.0)*std::exp(-(h-hgs)/100.0) + (1.0/(27.0*27.0))*std::pow(v,2.0)*(std::pow(h,10.0))*(5.94e-53)*(std::exp(-h/1000.0))+(2.7e-16)*std::exp(-h/1500.0))*(std::pow(h-hgs,5.0/6.0));

  return IntegralFunction;
}
#endif

} // namespace ns3
