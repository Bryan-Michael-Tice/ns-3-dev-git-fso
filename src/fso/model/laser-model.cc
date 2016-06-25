/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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


#include <ns3/log.h>
#include <cmath>
#include "laser-model.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LaserModel");

NS_OBJECT_ENSURE_REGISTERED (LaserModel);


LaserModel::LaserModel ()
{
}

LaserModel::~LaserModel ()
{
}

void 
LaserModel::SetBeamwidth (double beamwidthDegrees)
{ 
  NS_LOG_FUNCTION (this << beamwidthDegrees);
  m_beamwidthRadians = DegreesToRadians (beamwidthDegrees);
}

double
LaserModel::GetBeamwidth () const
{
  return m_beamwidthRadians;
}

void 
LaserModel::SetPhaseFrontRadius (double phaseFrontRadius)
{ 
  NS_LOG_FUNCTION (this << phaseFrontRadius);
  m_phaseFrontRadius = phaseFrontRadius;
}

double
LaserModel::GetPhaseFrontRadius () const
{
  return m_phaseFrontRadius;
}

void 
LaserModel::SetOutputPower (double outputPower)
{
  NS_LOG_FUNCTION (this << outputPower);
  m_outputPower = outputPower;
}

double 
LaserModel::GetOutputPower () const
{
  return m_outputPower;
}

TypeId
LaserModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::LaserModel")
    .SetParent<Object> ()
    .SetGroupName("Fso")
  ;
  return tid;
}



}

