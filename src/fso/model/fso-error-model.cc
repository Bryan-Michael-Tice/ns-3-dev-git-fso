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
 *
 * Modified by: Michael Di Perna <diperna.michael@gmail.com> 2016
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

int64_t
FsoErrorModel::AssignStreams (int64_t stream)
{
  return DoAssignStreams (stream);
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

void 
FsoDownLinkErrorModel::SetPhy (Ptr<FsoPhy> phy)
{
  m_phy = phy;
}

FsoDownLinkErrorModel::FsoDownLinkErrorModel ()
{
  NS_LOG_FUNCTION (this);
  m_logNormalDist = CreateObject<LogNormalRandomVariable> ();
  
  m_groundRefractiveIdx = 1.7e-14;  
  m_rmsWindSpeed = 21.0;
  m_updateIrradiance = true;

  m_turbulenceTimer.SetFunction (&FsoDownLinkErrorModel::SetIrradianceUpdate, this);
  //m_turbulenceTimer.SetArguments (m_updateIrradiance);
}

FsoDownLinkErrorModel::~FsoDownLinkErrorModel ()
{
  NS_LOG_FUNCTION (this);
}

void
FsoDownLinkErrorModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_phy = 0;
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
FsoDownLinkErrorModel::CalculateTurbulenceTimeConstant (double hTx, double hRx, double wavelength, double elevation)
{
  double result;
  double error;

  GWFunctionParameters params;
  params.A = m_groundRefractiveIdx;
  params.v = m_rmsWindSpeed;
  params.hgs = hRx;

  gsl_integration_workspace * w = gsl_integration_workspace_alloc (1000);

  gsl_function F;
  F.function = &GWIntegralFunction;
  F.params = &params;

  gsl_integration_qags (&F, hRx, hTx, 1e-10, 1e-10, 10000, w, &result, &error);
   
  gsl_integration_workspace_free (w);
  
  return ((2.729*std::pow(10.0,-8.0))*std::pow(wavelength*(1e6), 1.2)*std::pow(std::sin(elevation),0.6))/std::pow(result,0.6);//Should this be stored in a member variable?
}

double 
FsoDownLinkErrorModel::GetPacketSuccessRate (Ptr<Packet> packet, Ptr<FsoSignalParameters> fsoSignalParams)
{
  NS_LOG_DEBUG ("ErrorModel: packet size=" << packet->GetSize () << " bits, scintIndex=" << fsoSignalParams->scintillationIndex << ", meanIrradiance=" << fsoSignalParams->meanIrradiance); 
  CalculateNormRxIrradiance(fsoSignalParams);

  fsoSignalParams->normIrradiance = m_normalizedIrradiance;

  double rxIrradiance = fsoSignalParams->meanIrradiance*m_normalizedIrradiance;
  return rxIrradiance;
}

void 
FsoDownLinkErrorModel::CalculateNormRxIrradiance (Ptr<FsoSignalParameters> fsoSignalParams)
{
  if (m_updateIrradiance)
   {
     m_normalizedIrradiance = m_logNormalDist->GetValue(-0.5*fsoSignalParams->scintillationIndex, std::sqrt(fsoSignalParams->scintillationIndex));

     double greenwoodTimeConstant = CalculateTurbulenceTimeConstant(fsoSignalParams->txPhy->GetMobility()->GetPosition().z, m_phy->GetMobility()->GetPosition().z, fsoSignalParams->wavelength, (60.0*M_PI)/180.0);
     NS_LOG_DEBUG ("ErrorModel: Greenwood Time Constant=" << greenwoodTimeConstant << "s"); 
     if (m_turbulenceTimer.IsExpired ())
      { 
       m_turbulenceTimer.SetDelay (Seconds (greenwoodTimeConstant));
       m_turbulenceTimer.Schedule ();
      }
   }

  NS_LOG_DEBUG ("ErrorModel: Normalized Irradiance=" << m_normalizedIrradiance);  
}

void FsoDownLinkErrorModel::SetIrradianceUpdate ()
{
  NS_LOG_DEBUG ("ErrorModelTimer: Irradiance Update");  
  m_updateIrradiance = true;
}

int64_t
FsoDownLinkErrorModel::DoAssignStreams (int64_t stream)
{
  m_logNormalDist->SetStream (stream);
  return 1;
}


double
GWIntegralFunction (double h, void *params)
{
  double A = ((GWFunctionParameters *) params)->A;
  double v = ((GWFunctionParameters *) params)->v;
  double hgs = ((GWFunctionParameters *) params)->hgs;

  double GWIntegralFunction = (A*std::exp(-hgs/700.0)*std::exp(-(h-hgs)/100.0) + (1.0/(27.0*27.0))*std::pow(v,2.0)*(std::pow(h,10.0))*(5.94*std::pow(10.0,-53.0))*(std::exp(-h/1000.0))+(2.7*std::pow(10.0,-16.0))*std::exp(-h/1500.0))*(std::pow((2.8 + 30*std::exp(-1*std::pow((h - 9400.0)/4800.0,2.0))),5.0/3.0));

  return GWIntegralFunction;
}

} //namespace ns3
