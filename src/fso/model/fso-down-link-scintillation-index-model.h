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

#ifndef FSO_DOWN_LINK_SCINTILLATION_INDEX_MODEL_H
#define FSO_DOWN_LINK_SCINTILLATION_INDEX_MODEL_H


#include <ns3/object.h>
#include <ns3/mobility-model.h>
#include "fso-signal-parameters.h"
#include "fso-propagation-loss-model.h"
#ifdef HAVE_GSL
#include <gsl/gsl_math.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_cdf.h>
#endif


namespace ns3 {

#ifdef HAVE_GSL
typedef struct HVFunctionParameterType
{
  double A;
  double v;
  double hgs;
} HVFunctionParameters;

double HVIntegralFunction (double x, void *params);
#endif

/**
 * \ingroup fso
 *
 * \brief sets the scintillation index for a downlink channel
 *
 * Sets the scintillation index of a FsoSignalParameters object
 *
 */
class FsoDownLinkScintillationIndexModel : public FsoPropagationLossModel
{
public:
  FsoDownLinkScintillationIndexModel ();
  virtual ~FsoDownLinkScintillationIndexModel ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();

  //Inherited from FsoPropagationLossModel
  virtual void UpdateSignalParams (Ptr<FsoSignalParameters> fsoSignalParams, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b);

  /**
   * Calculate the scintillation index for the channel, valid for
   * downlink scenario under weak turbulent conditions
   *
   * \param f frequency in Hz
   * \param hTx height of transmitter in km
   * \param hRx height of receiver in km
   * \param zenith elevation in radians
   * \return the scintillation index
   */
  double CalculateScintillationIdx (double f, double hTx, double hRx, double zenith) const;

  void SetRmsWindSpeed (double rmsWindSpeed);
  double GetRmsWindSpeed () const;

  void SetGndRefractiveIdx (double gndRefractiveIdx);
  double GetGndRefractiveIdx () const;

protected:
  //Inherited from Object
  virtual void DoDispose ();
  

private:
  double m_rmsWindSpeed;        //!< The RMS wind speed in m/s
  double m_groundRefractiveIdx; //!< The index of refraction at ground level

};






} // namespace ns3

#endif /* FSO_DOWN_LINK_SCINTILLATION_INDEX_MODEL_H */
