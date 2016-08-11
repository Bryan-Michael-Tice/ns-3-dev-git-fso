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
 * fso applications. This class was created since PropagationLossModel 
 * uses CalcRxPower(), where as we would like to pass an FsoSignalParameters 
 * object to be modified by each fso propagation loss model. This class is similar 
 * to PropagationLossModel but uses UpdateSignalParams() in place of CalcRxPower().
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
   * \brief Enables a chain of loss models to act on the signal
   * \param next The next FsoPropagationLossModel to add to the chain
   *
   * This method of chaining propagation loss models only works commutatively
   * if the propagation loss of all models in the chain are independent
   * of each other.
   */
  void SetNext (Ptr<FsoPropagationLossModel> next);

  /**
   * \brief Gets the next FsoPropagationLossModel in the chain of loss models
   * that act on the signal.
   * \returns The next FsoPropagationLossModel in the chain
   *
   * This method of chaining propagation loss models only works commutatively
   * if the propagation loss of all models in the chain are independent
   * of each other.
   */
  Ptr<FsoPropagationLossModel> GetNext ();

  /**
   * Updates the signal parameters taking into account all the FsoPropagatinLossModel(s)
   * chained to the current one.
   *
   * \param fsoSignalParams is the signal parameters for the optical beam
   * \param a sender mobility
   * \param b receiver mobility
   */
  void UpdateSignalParams (Ptr<FsoSignalParameters> fsoSignalParams, 
                           Ptr<const MobilityModel> a, 
                           Ptr<const MobilityModel> b);

  /**
   * If this loss model uses objects of type RandomVariableStream,
   * set the stream numbers to the integers starting with the offset
   * 'stream'. Return the number of streams (possibly zero) that
   * have been assigned.  If there are FsoPropagationLossModels chained
   * together, this method will also assign streams to the
   * downstream models.
   *
   * \param stream
   * \return the number of stream indices assigned by this model
   */
  int64_t AssignStreams (int64_t stream);

private:

  /**
   * Subclasses must implement this; those not using random variables
   * can return zero
   */
  virtual int64_t DoAssignStreams (int64_t stream) = 0;

  Ptr<FsoPropagationLossModel> m_next; //!< Next propagation loss model in the list

  /**
   * \brief This purely virtual function that updates the signal parameters according to the        
   * implementation in the derived class
   * 
   *
   * \param fsoSignalParams is the signal parameters for the optical beam
   * \param a sender mobility
   * \param b receiver mobility
   *
   */
  virtual void DoUpdateSignalParams (Ptr<FsoSignalParameters> fsoSignalParams, 
                                     Ptr<const MobilityModel> a, 
                                     Ptr<const MobilityModel> b) = 0;

protected:
  //Inherited from Object
  virtual void DoDispose ();

};






} // namespace ns3

#endif /* FSO_PROPAGATION_LOSS_MODEL_H */
