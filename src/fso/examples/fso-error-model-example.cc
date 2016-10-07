/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 The Boeing Company
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
 * Author: Gary Pei <guangyu.pei@boeing.com>
 *
 * Modified by: Michael Di Perna <diperna.michael@gmail.com> 2016
 */

// This example is used to validate the error model used for the Fso Module.
//
// It ouputs plots of the Packet Success Rate versus the received power (Watts) for
// the downlink error model.

#include "ns3/core-module.h"
#include "ns3/fso-phy.h"
#include "ns3/optical-rx-antenna-model.h"
#include "ns3/fso-error-model.h"
#include "ns3/gnuplot.h"

#include <fstream>
#include <vector>
#include <cmath>

using namespace ns3;

int main (int argc, char *argv[])
{
  uint32_t packetSize = 1024;
  std::ofstream downlinkfile ("fso-downlink-packet-success-rate.plt");

  CommandLine cmd;
  cmd.AddValue ("PacketSize", "The packet size", packetSize);
  cmd.Parse (argc, argv);

  Gnuplot downlinkPlot = Gnuplot ("fso-downlink-packet-success-rate.eps");

  Ptr <FsoDownLinkErrorModel> errorModel = CreateObject<FsoDownLinkErrorModel> ();
  Ptr <OpticalRxAntennaModel> antenna = CreateObject<OpticalRxAntennaModel> ();
  Ptr <FsoPhy> phy = CreateObject<FsoPhy> (); 

  phy->SetAntennas (0, antenna);
  phy->SetErrorModel (errorModel);
  errorModel->SetPhy (phy);

  std::vector<double> power;
  power.push_back(1e-8);
  power.push_back(2e-8);
  power.push_back(3e-8);
  power.push_back(4e-8);
  power.push_back(5e-8);
  power.push_back(6e-8);
  power.push_back(7e-8);
  power.push_back(8e-8);
  power.push_back(9e-8);
  power.push_back(1e-7);
  power.push_back(2e-7);
  power.push_back(3e-7);
  power.push_back(4e-7);
  power.push_back(5e-7);


  Gnuplot2dDataset downlinkDataset ("Downlink");
  for (uint32_t i = 0; i < power.size (); i++)
    {
      std::cout << power[i] << " Watts" << std::endl;

      double ber = errorModel->CalculateBer (power[i]);
      double psr = 1.0 - errorModel->CalculatePacketLossProbability (ber, packetSize);
      downlinkDataset.Add (power[i], psr);
      downlinkPlot.AddDataset (downlinkDataset);
    }

  downlinkPlot.SetTerminal ("postscript eps color enh \"Times-BoldItalic\"");
  downlinkPlot.SetLegend ("Power(Watts)", "Packet Success Rate");
  downlinkPlot.GenerateOutput (downlinkfile);
  downlinkfile.close ();

}
