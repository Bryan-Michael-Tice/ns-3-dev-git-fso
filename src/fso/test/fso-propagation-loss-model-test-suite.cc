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
#include "ns3/constant-position-mobility-model.h"
#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FsoPropagationLossModelsTest");

// ===========================================================================
// This is a simple test to validate propagation loss models of ns-3 fso.
// See the chapter in the ns-3 testing and validation guide for more detail
// ===========================================================================
//
class FsoMeanIrradianceModelTestCase : public TestCase
{
public:
  FsoMeanIrradianceModelTestCase ();
  virtual ~FsoMeanIrradianceModelTestCase ();

private:
  virtual void DoRun (void);

  typedef struct {
    
    double m_distance;  //meters
    double m_frequency;  //Hz
    double m_txBeamRadius; //meters
    double m_txPhaseFrontRadius;// approximately equal to distance
    double m_meanIrradiance;//Watts/meter^2 
    double m_tolerance;
  } TestVector;

  TestVectors<TestVector> m_testVectors;
};

FsoMeanIrradianceModelTestCase::FsoMeanIrradianceModelTestCase ()
  : TestCase ("Check to see that the ns-3 mean irradiance model provides the correct power density"), m_testVectors ()
{
}

FsoMeanIrradianceModelTestCase::~FsoMeanIrradianceModelTestCase ()
{
}

void
FsoMeanIrradianceModelTestCase::DoRun (void)
{
 
  // We want to test the fso propagation loss model calculations for a pre-defined scenario 
  // and compare the results to those we have manually calculated
  // according to the model documentation, within the specificed tolerance
  
  double wavelength = 847e-9;//nm
  double frequency = 3e8/wavelength;
  TestVector testVector;
  
  testVector.m_distance = 707000;
  testVector.m_frequency = frequency;
  testVector.m_txBeamRadius = 0.06;
  testVector.m_txPhaseFrontRadius = 707000;
  testVector.m_meanIrradiance = 3.56696705922e-4;
  testVector.m_tolerance = 1e-9;  
  m_testVectors.Add (testVector);

  // Now, check that the received power values are expected

  Ptr<MobilityModel> a = CreateObject<ConstantPositionMobilityModel> (); 
  a->SetPosition (Vector (0,0,0));
  Ptr<MobilityModel> b = CreateObject<ConstantPositionMobilityModel> (); 

  Ptr<FriisPropagationLossModel> lossModel = CreateObject<FriisPropagationLossModel> (); 
  for (uint32_t i = 0; i < m_testVectors.GetN (); ++i)
    {
      testVector = m_testVectors.Get (i);
      b->SetPosition (Vector (0,0, testVector.m_distance));
        

      double rxDiffractiveBeamRadius = lossModel->CalculateDiffractiveBeamRadius (testVector.m_distance, testVector.m_frequency, testVector.m_txBeamRadius, testVector.m_txPhaseFrontRadius);
      double resultMeanIrradiance = lossModel->CalculateMeanIrradiance (testVector.m_txBeamradius, rxDiffractiveBeamRadius);

      NS_TEST_EXPECT_MSG_EQ_TOL (resultMeanIrradiance, testVector.m_meanIrradiance, testVector.m_tolerance, "Got unexpected mean irradiance at the receiver");
    }
}


class FsoDownLinkScintillationIndexModelTestCase : public TestCase
{
public:
  FsoDownLinkScintillationIndexModel ();
  virtual ~FsoDownLinkScintillationIndexModel ();

private:
  virtual void DoRun (void);

  typedef struct
  {
    double m_txHeight;  //meters
    double m_rxHeight;  //meters
    double m_frequency;  //Hz
    double m_zenith; //radians
    double m_scintIndex;
    double m_tolerance;
  } TestVector;

  TestVectors<TestVector> m_testVectors;
};

FsoDownLinkScintillationIndexModel::FsoDownLinkScintillationIndexModel ()
  : TestCase ("Check to see that the ns-3 TwoRayGround propagation loss model provides correct received power"),
    m_testVectors ()
{
}

FsoDownLinkScintillationIndexModel::~FsoDownLinkScintillationIndexModel ()
{
}

void
FsoDownLinkScintillationIndexModel::DoRun (void)
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
  testVector.m_zenith = 60.0*M_PI/180.0;
  testVector.m_tolerance = 1e-6;
  testVector.m_scintIndex = 0.16541409546;
  m_testVectors.Add (testVector);


  Ptr<FsoDownLinkScintillationIndexModel> lossModel = CreateObject<FsoDownLinkScintillationIndexModel> (); 

  lossModel->SetRmsWindSpeed(21.0);
  lossModel->SetGndRefractiveIdx(1.7e-14);

  for (uint32_t i = 0; i < m_testVectors.GetN (); ++i)
    {
      testVector = m_testVectors.Get (i);
      double resultScintIndex = lossModel->CalculateScintillationIdx (testVector.m_frequency, testVector.m_txHeight, testVector.m_rxHeight, testVector.m_zenith);

      NS_TEST_EXPECT_MSG_EQ_TOL (resultScintIndex, testVector.m_scintIndex, testVector.m_tolerance, "Got unexpected scintillation index");
    }
}

class FsoFreeSpaceLossModelTestCase : public TestCase
{
public:
  FsoFreeSpaceLossModelTestCase ();
  virtual ~FsoFreeSpaceLossModelTestCase ();

private:
  virtual void DoRun (void);

  typedef struct
  {
    double m_distance;//meters 
    double m_wavelength;//meters
    double m_tolerance;
    double m_loss;//dB
  } TestVector;

  TestVectors<TestVector> m_testVectors;
};

FsoFreeSpaceLossModelTestCase::FsoFreeSpaceLossModelTestCase ()
  : TestCase ("Check to see that the ns-3 TwoRayGround propagation loss model provides correct received power"),
    m_testVectors ()
{
}

FsoFreeSpaceLossModelTestCase::~FsoFreeSpaceLossModelTestCase ()
{
}

void
FsoFreeSpaceLossModelTestCase::DoRun (void)
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


class FsoPropagationLossModelsTestSuite : public TestSuite
{
public:
  FsoPropagationLossModelsTestSuite ();
};

FsoPropagationLossModelsTestSuite::PropagationLossModelsTestSuite ()
  : TestSuite ("propagation-loss-model", UNIT)
{
  AddTestCase (new FsoMeanIrradianceModelTestCase, TestCase::QUICK);
  AddTestCase (new FsoDownLinkScintillationIndexModel, TestCase::QUICK);
}

static FsoPropagationLossModelsTestSuite fsoPropagationLossModelsTestSuite;
