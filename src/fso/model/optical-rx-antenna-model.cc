/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 CTTC
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
 */


#include <ns3/log.h>
#include <ns3/double.h>
#include <cmath>

#include "optical-rx-antenna-model.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("OpticalRxAntennaModel");

NS_OBJECT_ENSURE_REGISTERED (OpticalRxAntennaModel);


TypeId 
OpticalRxAntennaModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::OpticalRxAntennaModel")
    .SetParent<AntennaModel> ()
    .SetGroupName("Antenna")
    .AddConstructor<OpticalRxAntennaModel> ()
    .AddAttribute ("Orientation",
                   "The angle (degrees) that expresses the orientation of the antenna on the x-y plane relative to the x axis",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&OpticalRxAntennaModel::SetOrientation,
                                       &OpticalRxAntennaModel::GetOrientation),
                   MakeDoubleChecker<double> (-360, 360))
    .AddAttribute ("ReceiverGain",
                   "The gain (dB) of the receiver.",
                   DoubleValue (20.0),
                   MakeDoubleAccessor (&OpticalRxAntennaModel::m_rxGain),
                   MakeDoubleChecker<double> ())
     
    .AddAttribute ("ApertureSize",
                   "The aperture size (meters^2) of the receiver.",
                   DoubleValue (0.25),
                   MakeDoubleAccessor (&OpticalRxAntennaModel::m_apertureSize),
                   MakeDoubleChecker<double> (0,100))
  ;
  return tid;
}

void 
OpticalRxAntennaModel::SetApertureSize (double apertureSize)
{ 
  NS_LOG_FUNCTION (this << apertureSize);
  m_apertureSize = apertureSize;
}

double
OpticalRxAntennaModel::GetApertureSize () const
{
  return m_apertureSize;
}

void 
OpticalRxAntennaModel::SetRxGain (double rxGain)
{ 
  NS_LOG_FUNCTION (this << rxGain);
  m_rxGain = rxGain;
}

double
OpticalRxAntennaModel::GetRxGain () const
{
  return m_rxGain;
}

void 
OpticalRxAntennaModel::SetOrientation (double orientationDegrees)
{
  NS_LOG_FUNCTION (this << orientationDegrees);
  m_orientationRadians = DegreesToRadians (orientationDegrees);
}

double
OpticalRxAntennaModel::GetOrientation () const
{
  return RadiansToDegrees (m_orientationRadians);
}


double 
OpticalRxAntennaModel::GetGainDb (Angles a)
{
  NS_LOG_FUNCTION (this << a);
  /*
  // azimuth angle w.r.t. the reference system of the antenna
  double phi = a.phi - m_orientationRadians;

  // make sure phi is in (-pi, pi]
  while (phi <= -M_PI)
    {
      phi += M_PI+M_PI;
    }
  while (phi > M_PI)
    {
      phi -= M_PI+M_PI;
    }

  NS_LOG_LOGIC ("phi = " << phi );

  double gainDb = -std::min (12 * pow (phi / m_beamwidthRadians, 2), m_maxAttenuation);

  NS_LOG_LOGIC ("gain = " << gainDb);
  return gainDb;
  */
  return 0.0;
}


}

