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
#include "fso-signal-parameters.h"

namespace ns3 {




/**
 * \ingroup fso
 *
 * \brief base class for fso propagation loss models
 *
 * Interface for propagation loss models to be adopted for
 * fso applications
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
   * \brief This purely virtual function that updates the signal parameters according to the        
   * implementation in the child class
   * 
   *
   * \param fsoSignalParams is the signal parameters for the optical beam
   * \param a sender mobility
   * \param b receiver mobility
   *
   */
  virtual void UpdateSignalParams (Ptr<FsoSignalParameters> fsoSignalParams, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b) = 0;

protected:
  //Inherited from Object
  virtual void DoDispose ();

};






} // namespace ns3

#endif /* FSO_PROPAGATION_LOSS_MODEL_H */
