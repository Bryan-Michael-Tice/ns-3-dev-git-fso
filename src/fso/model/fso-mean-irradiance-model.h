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

#ifndef FSO_MEAN_IRRADIANCE_MODEL_H
#define FSO_MEAN_IRRADIANCE_MODEL_H


#include <ns3/object.h>
#include <ns3/mobility-model.h>
#include "fso-signal-parameters.h"
#include "fso-propagation-loss-model.h"

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
class FsoMeanIrradianceModel : public FsoPropagationLossModel
{
public:
  FsoMeanIrradianceModel ();
  virtual ~FsoMeanIrradianceModel ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();

  //Inherited from FsoPropagationLossModel
  virtual void UpdateSignalParams (Ptr<FsoSignalParameters> fsoSignalParams, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b);

  /**
   * Calculate the mean irradiance
   *
   * @param txBeamRadius the beamwidth radius at the transmitter aperture
   * @param rxDiffractiveBeamRadius the diffractive beam radius at the receiver
   *
   */
  double CalculateMeanIrradiance (double txBeamRadius, double rxDiffractiveBeamRadius);

  /**
   * Calculate the diffractive beam radius at the receiver
   *
   * @param f frequency in Hz
   * @param d distance from tx to rx in m
   * @param txBeamRadius beam width radius at the transmitter at tx output aperture
   * @param txPhaseFrontRadius phase front radius of curvature at tx output aperture
   *
   */
  double CalculateDiffractiveBeamRadius (double d, double f, double txBeamRadius, double txPhaseFrontRadius);


protected:
  //Inherited from Object
  virtual void DoDispose ();
  

};






} // namespace ns3

#endif /* FSO_MEAN_IRRADIANCE_MODEL_H */
