
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

#include <cmath>

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
    , m_mioValidUntil()
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
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {

  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Ivrule::nextContainer(odcore::data::Container &a_container)
{
  if(!m_initialised){
    return;
  }
  if(a_container.getDataType() == opendlv::perception::Environment::ID()){
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
      m_mio = FindMio(listOfObjects);
    }
  }
}

opendlv::perception::Object Ivrule::FindMio(std::vector<opendlv::perception::Object> &a_listOfObjects)
{
  opendlv::perception::Object mio;
  std::vector<float> scoreList;

  for(auto object:a_listOfObjects){
    opendlv::model::Direction direction = object.getDirection();
    float azimuth = direction.getAzimuth();
    float distance = object.getDistance();
    std::vector<std::string> sources = object.getListOfSources();

    float score = 0;

    if(std::abs(azimuth) < m_mioAngleRange){
      score += 100;
    }
    score += (100-distance)*2;
    for(auto it:sources){
      score += 80;
    }

    if(distance > 50){
      score = 0;
    }
    scoreList.push_back(score);
  }


  
  // return NULL;
  odcore::data::TimeStamp now;
  return mio;
}

void Ivrule::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_mioAngleRange = kv.getValue<float>("knowledge-ivrule.mioAngleRange");
  m_mioDistanceRange = kv.getValue<float>("knowledge-ivrule.mioDistanceRange");
  m_initialised = true;
}

void Ivrule::tearDown()
{
}

} // ivrule
} // knowledge
} // opendlv

