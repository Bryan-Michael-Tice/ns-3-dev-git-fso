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

#ifndef FSO_PROPAGATION_LOSS_MODEL_H
#define FSO_PROPAGATION_LOSS_MODEL_H


#include <ns3/object.h>
#include <ns3/mobility-model.h>
#include <ns3/spectrum-value.h>

namespace ns3 {




/**
 * \ingroup spectrum
 *
 * \brief spectrum-aware propagation loss model
 *
 * Interface for propagation loss models to be adopted when
 * transmissions are modeled with a power spectral density by means of
 * the SpectrumValue class.
 *
 */
class FsoPropagationLossModel : public Object
{
public:
  FsoPropagationLossModel ();
  virtual ~FsoPropagationLossModel ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();

  /**
   * This method is to be called to calculate the irradiance at the
   * receiver.
   *
   * @param txBeamWidth is the beam width at the transmitter
   * in radians.
   *
   * @param a sender mobility
   * @param b receiver mobility
   *
   * @return the normalized irradiance at the receiver in watts/meter
   */
  double CalcRxIrradiance (double txBeamRadius,
                           Ptr<const MobilityModel> a,
                           Ptr<const MobilityModel> b) const;

protected:
  //Inherited from Object
  virtual void DoDispose ();

  /**
   * Calculate the scintillation index for the channel, valid for
   * downlink scenario under weak turbulent conditions
   *
   * @param f frequency in Hz
   * @param hGS height of ground station in m
   * @param hSat height of satellite in m
   * @param e elevation in radians
   *
   */
  void CalculateScintillationIdx (double f, double hGS, double hSat, double e) const;
  
  /**
   * Calculate the mean irradiance
   *
   * @param txBeamRadius the beam width radius at the transmitter at tx output aperture
   *
   */
  void CalculateMeanIrradiance (double txBeamRadius);

  /**
   * Calculate the diffractive beam radius at the receiver
   *
   * @param f frequency in Hz
   * @param d distance from tx to rx in m
   * @param txBeamRadius beam width radius at the transmitter at tx output aperture
   * @param txPhaseFrontRadius phase front radius of curvature at tx output aperture
   *
   */
  void CalculateDiffractiveBeamRadius (double f, double d, double txBeamRadius, double txPhaseFrontRadius);

private:
  double m_rmsWindSpeed;
  double m_groundRefractiveIdx;
  double m_scintIdx;
  double m_meanIrradiance;
  double m_rxDiffractiveBeamRadius;

};






} // namespace ns3

#endif /* FSO_PROPAGATION_LOSS_MODEL_H */
