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
 *
 * Modified by: Michael Di Perna <diperna.michael@gmail.com> 2016
 */


#include <ns3/log.h>
#include <ns3/double.h>
#include <cmath>

#include "laser-antenna-model.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LaserAntennaModel");

NS_OBJECT_ENSURE_REGISTERED (LaserAntennaModel);


TypeId 
LaserAntennaModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::LaserAntennaModel")
    .SetParent<AntennaModel> ()
    .SetGroupName("Antenna")
    .AddConstructor<LaserAntennaModel> ()
    .AddAttribute ("Beamwidth",
                   "The beamwidth (meters) at which the field amplitude falls to 1/e of that on the beam axis",
                   DoubleValue (60),
                   MakeDoubleAccessor (&LaserAntennaModel::SetBeamwidth,
                                       &LaserAntennaModel::GetBeamwidth),
                   MakeDoubleChecker<double> (0, 180))
    .AddAttribute ("Orientation",
                   "The angle (degrees) that expresses the orientation of the antenna on the x-y plane relative to the x axis",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&LaserAntennaModel::SetOrientation,
                                       &LaserAntennaModel::GetOrientation),
                   MakeDoubleChecker<double> (-360, 360))
    .AddAttribute ("PhaseFrontRadius", //MDP - to be removed
                   "The phase front radius of curvature (meters) of the beam.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&LaserAntennaModel::m_phaseFrontRadius),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Wavelength",
                   "The wavelength (meters) of the light.",
                   DoubleValue (847e-9),
                   MakeDoubleAccessor (&LaserAntennaModel::SetWavelength,
                                       &LaserAntennaModel::GetWavelength),
                   MakeDoubleChecker<double> (0.0, 1e-5))
    .AddAttribute ("TxPower",
                   "The transmit power (dB) of the laser.",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&LaserAntennaModel::SetTxPower,
                                       &LaserAntennaModel::GetTxPower),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Gain",
                   "The gain (dB) of the transmitter.",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&LaserAntennaModel::SetGain,
                                       &LaserAntennaModel::GetGain),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

void 
LaserAntennaModel::SetBeamwidth (double beamwidthMeters)
{ 
  NS_LOG_FUNCTION (this << beamwidthMeters);
  m_beamwidthMeters = beamwidthMeters;
}

double
LaserAntennaModel::GetBeamwidth () const
{
  return m_beamwidthMeters;
}

void 
LaserAntennaModel::SetOrientation (double orientationDegrees)
{
  NS_LOG_FUNCTION (this << orientationDegrees);
  m_orientationRadians = DegreesToRadians (orientationDegrees);
}

double
LaserAntennaModel::GetOrientation () const
{
  return RadiansToDegrees (m_orientationRadians);
}

void 
LaserAntennaModel::SetPhaseFrontRadius (double phaseFrontRadius)
{ 
  NS_LOG_FUNCTION (this << phaseFrontRadius);
  m_phaseFrontRadius = phaseFrontRadius;
}

double
LaserAntennaModel::GetPhaseFrontRadius () const
{
  return m_phaseFrontRadius;
}

void 
LaserAntennaModel::SetTxPower (double txPower)
{
  m_txPower = txPower;
}

double 
LaserAntennaModel::GetTxPower () const
{
  return m_txPower;
}

void 
LaserAntennaModel::SetGain (double gain)
{
  m_gain = gain;
}
  
double 
LaserAntennaModel::GetGain () const
{
  return m_gain;
}

double 
LaserAntennaModel::GetGainDb (Angles a)
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

