/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 CTTC
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


#ifndef FSO_SIGNAL_PARAMETERS_H
#define FSO_SIGNAL_PARAMETERS_H


#include <ns3/simple-ref-count.h>
#include <ns3/ptr.h>
#include <ns3/nstime.h>


namespace ns3 {

class FsoPhy;
class SpectrumValue;
class AntennaModel;

/**
 * \ingroup fso
 *
 * This struct provides the generic signal representation to be used by
 * optical signals.
 *
 * \note when inheriting from this class, make sure that the assignment operator and the copy constructor work properly, making deep copies if needed.
 */
struct FsoSignalParameters : public SimpleRefCount<FsoSignalParameters>
{
  /**
   * default constructor
   */
  FsoSignalParameters (); //MDP - shouldn't some default values be set in this?

  /**
   * destructor
   */
  virtual ~FsoSignalParameters ();

  /**
   * copy constructor
   */
  FsoSignalParameters (const FsoSignalParameters& p);

  /**
   * make a "virtual" copy of this class, where "virtual" refers to
   * the fact that if the actual object is a derived class of
   * SpectrumSignalParameters, then the copy is also a derived class
   * of the same type.
   * Each class inheriting from
   * FsoSignalParameters should override this method and use it
   * to call the copy constructor of the derived class.
   *
   * \return a copy of the (possibly derived) class
   */
  virtual Ptr<FsoSignalParameters> Copy ();

  /**
   * The duration of the packet transmission.
   */
  Time duration;

  /**
   * The FsoPhy instance that is making the transmission.
   */
  Ptr<FsoPhy> txPhy;

  /**
   * The AntennaModel instance that was used to transmit this signal.
   */
  Ptr<AntennaModel> txAntenna;
  
  /**
   * The wavelength of the signal (m).
   */
  double wavelength;

  /**
   * The frequency of the signal (Hz).
   */
  double frequency;

  /**
   * The symbol period (ns)
   */
  double symbolPeriod;

  /**
   * The power of the signal (dB).
   */
  double power;

  /**
   * The beamwidth of the beam at the transmitter (m).
   */
  double txBeamwidth;

  /**
   * The phase front radius of curvature of the beam at the transmitter ().
   */
  double txPhaseFrontRadius;

  /**
   * The scintillation index due to atmospheric refraction.
   */
  double scintillationIndex;

  /**
   * The mean irradiance of the signal at the receiver (Watts/m^2).
   */
  double meanIrradiance;
  
  /**
   * The path loss (free space) from the RX to TX (dB).
   */
  double pathLoss;

};


}




#endif /* FSO_SIGNAL_PARAMETERS_H */
