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


void SetIrradianceUpdate (bool &update)
{
  NS_LOG_DEBUG ("ErrorModelTimer: Irradiance Update");  
  update = true;
}

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

  m_turbulenceTimer.SetFunction (&SetIrradianceUpdate);
  m_turbulenceTimer.SetArguments (m_updateIrradiance);
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

  gsl_integration_qagiu (&F, hRx, hTx, 1e-7, 1000, w, &result, &error);
   
  gsl_integration_workspace_free (w);

  return (2.729e-8)*std::pow(wavelength, 1.2)*std::pow(std::sin(elevation),0.6)/std::pow(result,0.6);//Should this be stored in a member variable?
}

double 
FsoDownLinkErrorModel::GetPacketSuccessRate (Ptr<FsoSignalParameters> fsoSignalParams, uint32_t nbits)
{
  NS_LOG_DEBUG ("ErrorModel: packet size=" << nbits << " bits, scintIndex=" << fsoSignalParams->scintillationIndex << ", meanIrradiance=" << fsoSignalParams->meanIrradiance); 
  CalculateNormRxIrradiance(fsoSignalParams);

  double rxIrradiance = fsoSignalParams->meanIrradiance*m_normalizedIrradiance;

  return rxIrradiance;
}

void 
FsoDownLinkErrorModel::CalculateNormRxIrradiance (Ptr<FsoSignalParameters> fsoSignalParams)
{
  if (m_updateIrradiance)
   {
     m_normalizedIrradiance = m_logNormalDist->GetValue(-0.5*fsoSignalParams->scintillationIndex, std::sqrt(fsoSignalParams->scintillationIndex));

     double greenwoodTimeConstant = CalculateTurbulenceTimeConstant(fsoSignalParams->txPhy->GetMobility()->GetPosition().z, m_phy->GetMobility()->GetPosition().z, fsoSignalParams->wavelength, 60.0*M_PI/180.0);
     NS_LOG_DEBUG ("ErrorModel: Greenwood Time Constant=" << greenwoodTimeConstant << "s");  
     m_turbulenceTimer.SetDelay (Seconds (greenwoodTimeConstant));
     m_turbulenceTimer.Schedule ();
   }

  NS_LOG_DEBUG ("ErrorModel: Normalized Irradiance=" << m_normalizedIrradiance);  
}


#ifdef HAVE_GSL
double
GWIntegralFunction (double h, void *params)
{
  double A = ((GWFunctionParameters *) params)->A;
  double v = ((GWFunctionParameters *) params)->v;
  double hgs = ((GWFunctionParameters *) params)->hgs;
  double IntegralFunction = (A*std::exp(-hgs/700.0)*std::exp(-(h-hgs)/100.0) + (1.0/(27.0*27.0))*std::pow(v,2.0)*(std::pow(h,10.0))*(std::pow(5.94*10,-53.0))*(std::exp(-h/1000.0))+(std::pow(2.7*10,-16.0))*std::exp(-h/1500.0))*std::pow((2.8 + 30*std::exp(-std::pow((h - 9400.0)/4800.0,2.0))),5.0/3.0);

  return IntegralFunction;
}
#endif

} //namespace ns3
