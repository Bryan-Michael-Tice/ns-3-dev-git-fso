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

void 
FsoDownLinkScintillationIndexModel::UpdateSignalParams (FsoSignalParameters& fsoSignalParams, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (a->GetPosition().z >= b->GetPosition().z);
  
  double heightTx = (a->GetPosition().z)/1000.0;//in km
  double heightRx = (b->GetPosition().z)/1000.0;//in km  

  double elevation = 10.0*(M_PI/180.0);//MDP - This is fixed for now, Satellite mobility model should provide an elevation

  fsoSignalParams.scintillationIndex = CalculateScintillationIdx (fsoSignalParams.frequency, heightTx, heightRx, elevation);

}

double
FsoDownLinkScintillationIndexModel::CalculateScintillationIdx (double f, double hTx, double hRx, double e) const
{
  NS_LOG_FUNCTION (this);

  double result;
  double error;

  FunctionParameters params;
  params.A = m_groundRefractiveIdx;
  params.v = m_rmsWindSpeed;
  params.hgs = hRx;

  gsl_integration_workspace * w = gsl_integration_workspace_alloc (1000);

  gsl_function F;
  F.function = &IntegralFunction;
  F.params = &params;

  gsl_integration_qagiu (&F, hRx, hTx, 1e-7, 1000, w, &result, &error);

  double k = (2*M_PI)/((3e8)/f);//Wave number - 2*pi/wavelength
   
  gsl_integration_workspace_free (w);
  
  return (2.25*std::pow(k,7.0/6.0)*(1/std::pow(cos(e),11.0/6.0)))*result;
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
IntegralFunction (double h, void *params)
{
  double A = ((FunctionParameters *) params)->A;
  double v = ((FunctionParameters *) params)->v;
  double hgs = ((FunctionParameters *) params)->hgs;
  double IntegralFunction = (A*std::exp(-hgs/700.0)*std::exp(-(h-hgs)/100.0) + (1.0/(27.0*27.0))*std::pow(v,2.0)*(std::pow(h,10.0))*(std::pow(5.94*10,-53.0))*(std::exp(-h/1000.0))+(std::pow(2.7*10,-16.0))*std::exp(-h/1500.0))*(std::pow(h-hgs,5.0/6.0));

  return IntegralFunction;
}
#endif

} // namespace ns3
