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

#ifndef LASER_ANTENNA_MODEL_H
#define LASER_ANTENNA_MODEL_H


#include <ns3/object.h>
#include <ns3/antenna-model.h>

namespace ns3 {

/**
 * 
 * \brief  Laser model based on the parameters needed for the optical signal
 *
 */
class LaserAntennaModel : public AntennaModel
{
public:

  // inherited from Object
  static TypeId GetTypeId ();

  // inherited from AntennaModel
  virtual double GetGainDb (Angles a);


  // attribute getters/setters
  void SetBeamwidth (double beamwidthMeters);
  double GetBeamwidth () const;
  void SetOrientation (double orientationDegrees);
  double GetOrientation () const;
  void SetTxPower (double txPower);
  double GetTxPower () const;
  void SetGain (double gain);
  double GetGain () const;
  void SetWavelength (double wavelength);
  double GetWavelength () const;

private:

  double m_beamwidthMeters; //!< diameter of the beam in meters

  double m_orientationRadians;

  double m_txPower; //!< Transmit power in dB

  double m_gain; //!< Gain in dB

  double m_wavelength; //!< wavelength of light in meters
};



} // namespace ns3


#endif // LASER_ANTENNA_MODEL_H
