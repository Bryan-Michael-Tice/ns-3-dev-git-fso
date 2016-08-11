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


#include "fso-propagation-loss-model.h"

namespace ns3 {

//NS_LOG_COMPONENT_DEFINE ("FsoPropagationLossModel");

NS_OBJECT_ENSURE_REGISTERED (FsoPropagationLossModel);

FsoPropagationLossModel::FsoPropagationLossModel () : m_next (0)
{
}

FsoPropagationLossModel::~FsoPropagationLossModel ()
{
}

void
FsoPropagationLossModel::DoDispose ()
{
  
}

TypeId
FsoPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoPropagationLossModel")
    .SetParent<Object> ()
  ;
  return tid;
}

void
FsoPropagationLossModel::SetNext (Ptr<FsoPropagationLossModel> next)
{
  m_next = next;
}

Ptr<FsoPropagationLossModel>
FsoPropagationLossModel::GetNext ()
{
  return m_next;
}

void
FsoPropagationLossModel::UpdateSignalParams (Ptr<FsoSignalParameters> fsoSignalParams, 
                           Ptr<const MobilityModel> a, 
                           Ptr<const MobilityModel> b)
{
  DoUpdateSignalParams (fsoSignalParams, a, b);
  if (m_next != 0)
    {
      m_next->UpdateSignalParams (fsoSignalParams, a, b);
    }
}


int64_t
FsoPropagationLossModel::AssignStreams (int64_t stream)
{
  int64_t currentStream = stream;
  currentStream += DoAssignStreams (stream);
  if (m_next != 0)
    {
      currentStream += m_next->AssignStreams (currentStream);
    }
  return (currentStream - stream);
}


} // namespace ns3
