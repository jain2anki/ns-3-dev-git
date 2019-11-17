/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

// Define an object to create a jellyfish topology.

#ifndef JELLY_FISH_HELPER_H
#define JELLY_FISH_HELPER_H

#include <string>
#include "ns3/csma-module.h"

namespace ns3 {

class JellyFishHelper
{
public:
  /**
   * Create a JellyFishHelper in order to easily create
   * jellyfish topologies using Csma links
   * \param nTopOfRackSwitch total number of switches

   * \param nPort the number of ports in a switch
   *
   * \param nServer the number of servers connected to one switch

   * \param switchToSwitch the csma links between two different switches

   * \param switchToServer the csma links between switch and server
   */
  JellyFishHelper(uint32_t nTopOfRackSwitch, uint32_t nPort, uint32_t nServer,
                              CsmaHelper switchToSwitch, CsmaHelper switchToServer);
  ~JellyFishHelper ();
  uint32_t  SwitchCount () const;

/**

*
* \param nTopOfRackSwitch total number of switches
*
* \param nPort the number of ports in a switch

* \param switchPorts the number of ports on one switch which connects to other switches

*/
std::vector<std::pair<int,int>> edgeSet(int nTopOfRackSwitch, int nPort, int switchPorts);
private:
  NodeContainer          m_servers;
  NetDeviceContainer     server_deices;
  NodeContainer          m_switches;
  NetDeviceContainer     switch_devices;
};

} // namespace ns3
#endif /* JELLY_FISH_HELPER_H */
