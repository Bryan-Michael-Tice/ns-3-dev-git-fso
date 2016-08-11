/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *
 * Modified by: Michael Di Perna <diperna.michael@gmail.com> 2016
 */

#ifndef ERROR_RATE_MODEL_H
#define ERROR_RATE_MODEL_H

#include <stdint.h>
#include "fso-phy.h"
#include "ns3/object.h"
#include "ns3/timer.h"
#include "ns3/random-variable-stream.h"
#include "ns3/mobility-model.h"
#include "fso-signal-parameters.h"
#ifdef HAVE_GSL
#include <gsl/gsl_math.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_cdf.h>
#endif

namespace ns3 {

#ifdef HAVE_GSL
typedef struct GWFunctionParameterType
{
  double A;
  double v;
  double hgs;
} GWFunctionParameters;

double GWIntegralFunction (double x, void *params);
#endif

/**
 * \ingroup fso
 * \brief The interface for Fso's error models.
 *
 */
class FsoErrorModel : public Object
{
public:
  static TypeId GetTypeId (void);

  /**
   * A pure virtual method that must be implemented in the subclass.
   * This method returns the probability that a packet will be
   * successfully received by the PHY.
   *
   * \param packet pointer to the packet
   * \param fsoSignalParams pointer to the optical signal parameters
   *
   * \return probability of successfully receiving the packet
   */
  virtual double GetPacketSuccessRate (Ptr<Packet> packet, Ptr<FsoSignalParameters> fsoSignalParams) = 0;
};

/**
 * \ingroup fso
 * \brief Error model computes the bit error rate based on properties of the receiver
 * and the irradiance at the receiver.
 *
 * The error model computes a bit error rate based on the constants of the receiver
 * and computes the irradiance received based on the scintillation index and mean irradiance.
 * The received irradiance is a random variable from a log normal distribution.   
 *
 * The greenwood time constant (time for which the atmospheric
 * turbulence can be considered constant) is computed to schedule 
 * an update for the received irradiance. 
 */
class FsoDownLinkErrorModel : public FsoErrorModel
{
public:

  static TypeId GetTypeId (void);

  FsoDownLinkErrorModel ();
  ~FsoDownLinkErrorModel ();

  void SetPhy (Ptr<FsoPhy> phy);

  /**
   * \return the snr
   */
  double CalculateMeanSnr () const;

  /**
   * \return the ber
   */
  double CalculateMeanBer () const;
  
  /**
   * \brief Calculate the normalized irradiance at the receiver
   * 
   * The normalized irradiance is stored in a member variable and is only re-calculated
   * when the  
   *
   * \param fsoSignalParams the signal parameters
   */
  void CalculateNormRxIrradiance (Ptr<FsoSignalParameters> fsoSignalParams);

  /**
   * \brief Calculate the greenwood time constant 
   *
   * \param hTx height of transmitter in meters
   * \param hRx height of receiver in meters
   * \param wavelength the wavelength in meters
   * \param elevation the elevation in radians
   * \return the time constant of the turbulence
   */
  double CalculateTurbulenceTimeConstant(double hTx, double hRx, double wavelength, double elevation);
  
  //inherited from FsoErrorModel
  virtual double GetPacketSuccessRate (Ptr<Packet> packet, Ptr<FsoSignalParameters> fsoSignalParams);
  
  /**
   * \brief Callback function to set boolean flag to update the irradiance
   *
   */
  void SetIrradianceUpdate ();

private:
  Ptr<LogNormalRandomVariable> m_logNormalDist; //!< Pointer to the log normal random variable
  Ptr<FsoPhy> m_phy; //!< Pointer to the Phy

  double m_groundRefractiveIdx; //!< Index of refraction at ground level
  double m_rmsWindSpeed;        //!< The RMS wind speed in m/s
  double m_normalizedIrradiance;//!< The normalized irradiance at the receiver (unitless) 

  bool m_updateIrradiance;      //!< Denotes if the irradiance at the receiver should be updated
  Timer m_turbulenceTimer;      //!< Timer related to the greenwood constant for irradiance calculation
};

} //namespace ns3

#endif /* ERROR_RATE_MODEL_H */

