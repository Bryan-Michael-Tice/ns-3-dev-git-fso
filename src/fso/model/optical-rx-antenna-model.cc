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
     
    .AddAttribute ("ApertureDiameter",
                   "The aperture size (meters^2) of the receiver.",
                   DoubleValue (0.25),
                   MakeDoubleAccessor (&OpticalRxAntennaModel::m_apertureDiameter),
                   MakeDoubleChecker<double> (0,1000))
    .AddAttribute ("CharacteristicPower",
                   "The characteristic power (Watts) of the receiver.",
                   DoubleValue (4.19e-9),
                   MakeDoubleAccessor (&OpticalRxAntennaModel::m_characteristicPower),
                   MakeDoubleChecker<double> (0.0,1.0))
     
    .AddAttribute ("FormFactor",
                   "The form factor of the receiver.",
                   DoubleValue (0.46),
                   MakeDoubleAccessor (&OpticalRxAntennaModel::m_formFactor),
                   MakeDoubleChecker<double> (0.0,1.0))
  ;
  return tid;
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
OpticalRxAntennaModel::GetGain () const
{
  return m_rxGain;
}

double 
OpticalRxAntennaModel::GetApertureDiameter () const
{
  return m_apertureDiameter;
}

double 
OpticalRxAntennaModel::GetCharacteristicPower () const
{
  return m_characteristicPower;
}


double 
OpticalRxAntennaModel::GetFormFactor () const
{
  return m_formFactor;
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

