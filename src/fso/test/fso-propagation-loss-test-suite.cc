/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 The Boeing Company
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
 * Modified by: Michael Di Perna <diperna.michael@gmail.com> 2016
 */

#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/test.h"
#include "ns3/config.h"
#include "ns3/double.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/fso-mean-irradiance-model.h"
#include "ns3/fso-down-link-scintillation-index-model.h"
#include "ns3/fso-free-space-loss-model.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/simulator.h"
#include <cmath>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FsoPropagationLossTest");

// ===========================================================================
// This is a simple test to validate propagation loss models of ns-3 fso.
// See the chapter in the ns-3 testing and validation guide for more detail
// ===========================================================================
//

/**
 * \ingroup fso
 *
 * \brief Test case for the mean irradiance at the receiver
 *
 * The test cases are generated from the Matlab script in the test/references folder.
 * The results from the Matlab script are compared with the result from the fso module.
 *
 */
class FsoMeanIrradianceTestCase : public TestCase
{
public:
  FsoMeanIrradianceTestCase ();//!< default constructor
  virtual ~FsoMeanIrradianceTestCase ();//!< virtual destructor

private:
  virtual void DoRun (void);//!< run test

  typedef struct {
    
    double m_distance;          //!< link distance in meters
    double m_frequency;         //!< frequency of optical signal in Hertz
    double m_txBeamRadius;      //!< the beam radius at the transmitter in meters
    double m_rxPhaseFrontRadius;//!< the phase front radius of curvature in meters
    double m_meanIrradiance;    //!< the mean irradiance at the receiver in Watts/meter^2 
    double m_tolerance;         //!< the tolerance when comparing the calculated value to the sim
  } TestVector;

  TestVectors<TestVector> m_testVectors; //!< vector of tests
};

FsoMeanIrradianceTestCase::FsoMeanIrradianceTestCase ()
  : TestCase ("Check to see that the ns-3 mean irradiance model provides the correct power density"),
    m_testVectors ()
{
}

FsoMeanIrradianceTestCase::~FsoMeanIrradianceTestCase ()
{
}

void
FsoMeanIrradianceTestCase::DoRun (void)
{
  // We want to test the fso propagation loss model calculations for a pre-defined scenario 
  // and compare the results to those we have manually calculated
  // according to the model documentation, within the specificed tolerance
  
  double wavelength = 847e-9;//meters
  double frequency = 3e8/wavelength;//Hertz
  TestVector testVector;
  
  testVector.m_distance = 707000;
  testVector.m_frequency = frequency;
  testVector.m_txBeamRadius = 0.06;
  testVector.m_rxPhaseFrontRadius = 707000;
  testVector.m_meanIrradiance = 3.56696705922e-4;
  testVector.m_tolerance = 1e-9;  

  m_testVectors.Add (testVector);

  // Now, check that the received power values are expected

  Ptr<MobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
  a->SetPosition (Vector (0,0,0));
  Ptr<MobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();

  Ptr<FsoMeanIrradianceModel> lossModel = CreateObject<FsoMeanIrradianceModel> ();
  for (uint32_t i = 0; i < m_testVectors.GetN (); ++i)
    {
      testVector = m_testVectors.Get (i);
      b->SetPosition (Vector (0,0, testVector.m_distance));


      double rxDiffractiveBeamRadius = lossModel->CalculateDiffractiveBeamRadius (testVector.m_distance, testVector.m_frequency, testVector.m_txBeamRadius, testVector.m_rxPhaseFrontRadius);
      double resultMeanIrradiance = lossModel->CalculateMeanIrradiance (testVector.m_txBeamRadius, rxDiffractiveBeamRadius);

      NS_TEST_EXPECT_MSG_EQ_TOL (resultMeanIrradiance, testVector.m_meanIrradiance, testVector.m_tolerance, "Got unexpected mean irradiance at the receiver");
    }
}

/**
 * \ingroup fso
 *
 * \brief Test case for the downlink scintillation index of the atmosphere
 *
 * The test cases are generated from the Matlab script in the test/references folder.
 * The results from the Matlab script are compared with the result from the fso module.
 *
 */
class FsoDownLinkScintillationIndexTestCase : public TestCase
{
public:
  FsoDownLinkScintillationIndexTestCase ();
  virtual ~FsoDownLinkScintillationIndexTestCase ();

private:
  virtual void DoRun (void);

  typedef struct
  {
    double m_txHeight;  //!< height of the transmitter in meters
    double m_rxHeight;  //!< height of the receiver in meters
    double m_frequency; //!< frequency of the optical signal in Hertz
    double m_zenith;    //!< zenith angle in radians
    double m_scintIndex;//!< scintillation index of the atmosphere
    double m_tolerance; //!< the tolerance when comparing the calculated value to the sim
  } TestVector;

  TestVectors<TestVector> m_testVectors;//!< vector of tests
};

FsoDownLinkScintillationIndexTestCase::FsoDownLinkScintillationIndexTestCase ()
  : TestCase ("Check to see that the ns-3 scintillation index model provides the scintilliation index"),
    m_testVectors ()
{
}

FsoDownLinkScintillationIndexTestCase::~FsoDownLinkScintillationIndexTestCase ()
{
}

void
FsoDownLinkScintillationIndexTestCase::DoRun (void)
{

  // We want to test the fso propagation loss model calculations for a pre-defined scenario 
  // and compare the results to those we have manually calculated
  // according to the model documentation, within the specificed tolerance
  
  double wavelength = 847e-9;
  double frequency = 3e8/wavelength;
  TestVector testVector;

  testVector.m_txHeight = 707000;
  testVector.m_rxHeight = 0;
  testVector.m_frequency = frequency;
  testVector.m_zenith = (60.0 * M_PI)/180.0;
  testVector.m_tolerance = 1e-6;
  testVector.m_scintIndex = 0.16541409546;

  m_testVectors.Add (testVector);

  Ptr<FsoDownLinkScintillationIndexModel> lossModel = CreateObject<FsoDownLinkScintillationIndexModel> (); 

  lossModel->SetRmsWindSpeed (21.0);
  lossModel->SetGndRefractiveIdx (1.7e-14);

  for (uint32_t i = 0; i < m_testVectors.GetN (); ++i)
    {
      testVector = m_testVectors.Get (i);
      double resultScintIndex = lossModel->CalculateScintillationIdx (testVector.m_frequency, testVector.m_txHeight, testVector.m_rxHeight, testVector.m_zenith);

      NS_TEST_EXPECT_MSG_EQ_TOL (resultScintIndex, testVector.m_scintIndex, testVector.m_tolerance, "Got unexpected scintillation index");
    }
}

/**
 * \ingroup fso
 *
 * \brief Test case for the free space path loss
 *
 * The test cases are generated from the Matlab script in the test/references folder.
 * The results from the Matlab script are compared with the result from the fso module.
 *
 */
class FsoFreeSpaceLossTestCase : public TestCase
{
public:

  FsoFreeSpaceLossTestCase ();
  virtual ~FsoFreeSpaceLossTestCase ();

private:
  virtual void DoRun (void);

  typedef struct
  {
    double m_distance;  //!< link distance in meters 
    double m_wavelength;//!< wavelength of the optical signal in meters
    double m_loss;      //!< free space path loss in dB
    double m_tolerance; //!< the tolerance when comparing the calculated value to the sim
  } TestVector;

  TestVectors<TestVector> m_testVectors;//!< vector of tests
};

FsoFreeSpaceLossTestCase::FsoFreeSpaceLossTestCase ()
  : TestCase ("Check to see that the ns-3 free space loss model provides the correct loss"),
    m_testVectors ()
{
}

FsoFreeSpaceLossTestCase::~FsoFreeSpaceLossTestCase ()
{
}

void
FsoFreeSpaceLossTestCase::DoRun (void)
{

  // We want to test the fso propagation loss model calculations for a pre-defined scenario 
  // and compare the results to those we have manually calculated
  // according to the model documentation, within the specificed tolerance
  
  TestVector testVector;

  testVector.m_distance = 707000;
  testVector.m_wavelength = 847e-9;
  testVector.m_tolerance = 1e-6;
  testVector.m_loss = 260.41;

  m_testVectors.Add (testVector);

  Ptr<FsoFreeSpaceLossModel> lossModel = CreateObject<FsoFreeSpaceLossModel> (); 

  for (uint32_t i = 0; i < m_testVectors.GetN (); ++i)
    {
      testVector = m_testVectors.Get (i);
      double resultLoss = lossModel->CalculateFreeSpaceLoss (testVector.m_distance, testVector.m_wavelength);

      NS_TEST_EXPECT_MSG_EQ_TOL (resultLoss, testVector.m_loss, testVector.m_tolerance, "Got unexpected free space loss");
    }
}


class FsoPropagationLossTestSuite : public TestSuite
{
public:
  FsoPropagationLossTestSuite ();
};

FsoPropagationLossTestSuite::FsoPropagationLossTestSuite ()
  : TestSuite ("propagation-loss-model", UNIT)
{
  AddTestCase (new FsoMeanIrradianceTestCase, TestCase::QUICK);
  AddTestCase (new FsoDownLinkScintillationIndexTestCase, TestCase::QUICK);
  AddTestCase (new FsoFreeSpaceLossTestCase, TestCase::QUICK);
}

static FsoPropagationLossTestSuite fsoPropagationLossTestSuite;
