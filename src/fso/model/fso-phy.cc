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

#include "fso-phy.h"
#include <ns3/net-device.h>
#include <ns3/mobility-model.h>
#include <ns3/fso-channel.h>
#include <ns3/log.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FsoPhy");

NS_OBJECT_ENSURE_REGISTERED (FsoPhy);


TypeId
FsoPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FsoPhy")
    .SetParent<Object> ()
    .SetGroupName ("Fso")
  ;
  return tid;
}


FsoPhy::~FsoPhy ()
{
  NS_LOG_FUNCTION (this);
}

void 
FsoPhy::DoDispose ()
{

}


} // namespace
