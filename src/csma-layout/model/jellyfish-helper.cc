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

// ns3 includes

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/jellyfish-helper.h"


namespace ns3 {
JellyFishHelper::JellyFishHelper(uint32_t nTopOfRackSwitch, uint32_t nPort, uint32_t nServer,
                              CsmaHelper switchToSwitch, CsmaHelper switchToServer) {
  
  m_switches.Create(nTopOfRackSwitch);
  m_servers.Create(nTopOfRackSwitch * nServer);

  std::vector<std::pair<int,int>> edges = edgeSet(nTopOfRackSwitch, nPort, nPort - nServer);

  
  uint32_t i, j;
  for(i = 0; i < edges.size(); ++i) {
    std::pair<uint32_t,uint32_t> edge = edges[i];
    NodeContainer temp;
    temp.Add(m_switches.Get(edge.first));
    temp.Add(m_switches.Get(edge.second));
    switchToSwitch.Install(temp);
  }
  for(i = 0; i < nTopOfRackSwitch; ++i) {
    for(j = 0; j < nServer; ++j) {
      NodeContainer temp;
      temp.Add(m_switches.Get(i));
      temp.Add(m_servers.Get(i*nServer+j));
      switchToServer.Install(temp);
    }
  }
}

JellyFishHelper::~JellyFishHelper() {

}

std::vector<std::pair<int,int>> JellyFishHelper::edgeSet(int nTopOfRackSwitch, int nPort, int switchPorts) {
  int switches = nTopOfRackSwitch;
  std::vector<std::vector<int> > adj(switches,std::vector<int>(switches,0));
  std::vector<int> vec; // vector for storing switch numbers 
  int i;
  for(i=0;i<switches;i++)
    vec.push_back(i);
  std::vector<int> randomSwitches;
  srand(time(0));

  // forming a clique by randomly selecting switchPorts+1 number of switches  

  for(i=0;i<=switchPorts;i++)
  {
    int siz = vec.size();
    int index = rand()%siz;
    randomSwitches.push_back(vec[index]);
    vec.erase(vec.begin()+index);
  }

  std::vector<int> remainingSwitches = vec; // storing left over switches

  //storing edges of the clique 
  std::vector<std::pair<int,int> >edgeSet;
  for(int i=0;i<=switchPorts;i++)
  {
    for(int j=i+1;j<=switchPorts;j++)
    {
      edgeSet.push_back(std::make_pair(randomSwitches[i],randomSwitches[j]));
      adj[randomSwitches[i]][randomSwitches[j]]=adj[randomSwitches[j]][randomSwitches[i]]=1;
    }
  }

  int remainingPort=-1; // stores remaining port on any switch if any


  // adding remaining switches to the clique
  for(int i=0;i<(int)remainingSwitches.size();i++)
  {
    std::vector<std::pair<int,int> >additionalEdges;
    int currPorts = switchPorts;
    
    // if remainingPort is there, join that switch with current switch
    if(remainingPort!=-1)
    {
      additionalEdges.push_back(std::make_pair(remainingPort,remainingSwitches[i]));
      currPorts--;
      adj[remainingPort][remainingSwitches[i]]=1;
    }

    //reamove an edge and store the additional two edges 

    while(currPorts>1)
    {
      int siz = edgeSet.size();
      int index = rand()%siz;
      int node1 = edgeSet[index].first;
      int node2 = edgeSet[index].second;
      if((adj[node1][remainingSwitches[i]])||(adj[node2][remainingSwitches[i]]))
        continue;
      additionalEdges.push_back(std::make_pair(node1,remainingSwitches[i]));
      additionalEdges.push_back(std::make_pair(node2,remainingSwitches[i]));
      adj[node1][node2]=0;
      edgeSet.erase(edgeSet.begin()+index);
      adj[node1][remainingSwitches[i]] = adj[node2][remainingSwitches[i]] = 1;
      currPorts -= 2;
    }

    // update the remaining Port switch if any port is left

    if(currPorts%2 == 1)
    {
      remainingPort = remainingSwitches[i];
    }
    else
    {
      remainingPort=-1;
      //If no port is left, set reamining port to -1
    }
    // Move the additional edges to the edge set

    for(int j=0;j<(int)additionalEdges.size();j++)
    {
      edgeSet.push_back(additionalEdges[j]);
    }
  }
  return edgeSet;
}

}





