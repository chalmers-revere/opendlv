
/**
 * Copyright (C) 2015 Chalmers REVERE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */


#include "ivrule/ivrule.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

namespace opendlv {
namespace knowledge {
namespace ivrule {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Ivrule::Ivrule(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "knowledge-ivrule")
    , m_initialised(false)
    , m_mioAngleRange()
    , m_mioDistanceRange()
    , m_memoryDuration()
    , m_desiredAngularSize()
    , m_desiredOpticalFlow()
    , m_mioValidUntil(0,0)
    , m_mio()
{
}

Ivrule::~Ivrule()
{
}

/**
 * Receives Environment object from scene.
 * Sends desired behaviour to the actuation layer.
 */

 odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Ivrule::body()
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    odcore::data::TimeStamp now;

    opendlv::sensation::DesiredOpticalFlow dof(m_desiredOpticalFlow);
    odcore::data::Container containerDof(dof);
    getConference().send(containerDof);



    if(m_mioValidUntil.toMicroseconds() - now.toMicroseconds() > 0) {
      // Steer to mio
      opendlv::sensation::DesiredDirectionOfMovement ddom(m_mio.getDirection());
      odcore::data::Container containerDdom(ddom);
      getConference().send(containerDdom);

      std::cout << "Sent ddom. Azimuth: " << m_mio.getDirection().getAzimuth() << std::endl;

      // opendlv::perception::ObjectDesiredAngularSize das(m_mio.getDirection(),m_mio.getAngularSize(),m_desiredAngularSize);
      // odcore::data::Container containerDas(das);
      // getConference().send(containerDas);

      std::cout << "Sent daf. Desired angular size: " << m_desiredAngularSize << " Currently: " << m_mio.getAngularSize() << std::endl;

    }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Ivrule::nextContainer(odcore::data::Container &a_container)
{
  if(!m_initialised) {
    return;
  }
  if(a_container.getDataType() == opendlv::perception::Environment::ID()) {
    opendlv::perception::Environment message = a_container.getData<opendlv::perception::Environment>();
    ReadEnvironment(message);
  }
}

void Ivrule::ReadEnvironment(opendlv::perception::Environment &a_environment)
{
  odcore::data::TimeStamp now;
  odcore::data::TimeStamp environmentValidUntil = a_environment.getValidUntil();
  //1 second as valid data
  if(now.toMicroseconds()-environmentValidUntil.toMicroseconds() < 0){
    std::vector<opendlv::perception::Object> listOfObjects = a_environment.getListOfObjects();
    if(listOfObjects.size() > 0){
      FindMio(listOfObjects);
    }
  }
}

void Ivrule::FindMio(std::vector<opendlv::perception::Object> &a_listOfObjects)
{
  opendlv::perception::Object mio;
  std::vector<float> scoreList;

  for(auto object:a_listOfObjects){
    opendlv::model::Direction direction = object.getDirection();
    float azimuth = direction.getAzimuth();
    float distance = object.getDistance();
    std::vector<std::string> sources = object.getListOfSources();

    float score = 0;

    score += (100-distance)*2;
    for(auto it:sources){
      score += 80;
    }

    if(distance > m_mioDistanceRange || std::abs(azimuth) > m_mioAngleRange){
      score = 0;
    }
    scoreList.push_back(score);
  }
  auto highestScore = std::max_element(scoreList.begin(),scoreList.end());
  if(*highestScore > 0){
    auto winnerIndex = std::distance(scoreList.begin(), highestScore)-1;
    m_mio = a_listOfObjects[winnerIndex];

    std::cout << "Winner index: " << winnerIndex << " Score: " << *highestScore << std::endl;
    odcore::data::TimeStamp now;
    m_mioValidUntil = odcore::data::TimeStamp(now.getSeconds()+m_memoryDuration,now.getFractionalMicroseconds());

  }
}

void Ivrule::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_mioAngleRange = static_cast<float>(kv.getValue<double>("knowledge-ivrule.mioAngleRange")*opendlv::Constants::DEG2RAD);
  m_mioDistanceRange = kv.getValue<float>("knowledge-ivrule.mioDistanceRange");
  m_memoryDuration = kv.getValue<uint8_t>("knowledge-ivrule.memoryDuration");
  m_desiredAngularSize = static_cast<float>(kv.getValue<double>("knowledge-ivrule.desiredAngularSize")*opendlv::Constants::DEG2RAD);
  m_desiredOpticalFlow = kv.getValue<float>("knowledge-ivrule.desiredOpticalFlow");
  m_initialised = true;
  std::cout << "Setup complete." << std::endl;
}

void Ivrule::tearDown()
{
}

} // ivrule
} // knowledge
} // opendlv

