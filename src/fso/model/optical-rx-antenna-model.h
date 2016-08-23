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

#ifndef OPTICAL_RX_ANTENNA_MODEL_H
#define OPTICAL_RX_ANTENNA_MODEL_H


#include <ns3/object.h>
#include <ns3/antenna-model.h>

namespace ns3 {

/**
 * 
 * \brief  Optical receiver antenna model which contains parameters required to compute the received power
 *
 *
 */
class OpticalRxAntennaModel : public AntennaModel
{
public:

  // inherited from Object
  static TypeId GetTypeId ();

  // inherited from AntennaModel
  virtual double GetGainDb (Angles a);


  // attribute getters/setters
  void SetOrientation (double orientationDegrees);
  double GetOrientation () const;
  /**
   * Get the receiver gain
   *
   * \return the receiver gain
   */
  double GetRxGain () const;

  /**
   * Get the diameter of the aperture
   *
   * \return the diameter of the aperture
   */
  double GetApertureDiameter () const;

  /**
   * Get the characteristic power of the receiver
   *
   * \return the characteristic power
   */
  double GetCharacteristicPower () const;

  /**
   * Get the form factor of the receivere
   *
   * \return the form factor
   */
  double GetFormFactor () const;


private:

  double m_apertureDiameter;    //!< The diameter of the receiver in meters

  double m_orientationRadians;  //!< The orientation of the receiver in radians

  double m_rxGain;              //!< The receiver gain in dB

  double m_characteristicPower; //!< The characteristic power in Watts 

  double m_formFactor;          //!< Form factor of the optical receiver
};



} // namespace ns3


#endif // OPTICAL_RX_ANTENNA_MODEL_H
