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

#ifndef FSO_FREE_SPACE_LOSS_MODEL_H
#define FSO_FREE_SPACE_LOSS_MODEL_H


#include <ns3/object.h>
#include <ns3/mobility-model.h>
#include "fso-signal-parameters.h"
#include "fso-propagation-loss-model.h"

namespace ns3 {




/**
 * \ingroup fso
 *
 * \brief free space propagation loss model
 *
 * Based on the equation for free space path loss for electromagnetic waves
 * 20*log(4*PI*distance/wavelength)
 *
 */
class FsoFreeSpaceLossModel : public FsoPropagationLossModel
{
public:
  FsoFreeSpaceLossModel ();
  virtual ~FsoFreeSpaceLossModel ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();

  //Inherited from FsoPropagationLossModel
  virtual void UpdateSignalParams (Ptr<FsoSignalParameters> fsoSignalParams, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b);

  /**
   * \brief Calculate the free space path loss (assuming far field) in dB
   * 
   *
   * \param f frequency in Hz
   * \param d distance from tx to rx in m
   *
   */
  double CalculateFreeSpaceLoss (double d, double wavelength);


protected:
  //Inherited from Object
  virtual void DoDispose ();
};






} // namespace ns3

#endif /* FSO_FREE_SPACE_LOSS_MODEL_H */
