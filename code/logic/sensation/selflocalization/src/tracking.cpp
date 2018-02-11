/**
 * Copyright (C) 2017 Chalmers Revere
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

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "opendavinci/odcore/data/TimeStamp.h"

#include "tracking.hpp"


namespace opendlv {
namespace logic {
namespace sensation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Tracking::Tracking(std::shared_ptr<Selflocalization> pSelfLocalization, odcore::base::KeyValueConfiguration kv/*, std::shared_ptr<OrbVocabulary> m_pVocabulary, std::shared_ptr<KeyFrameDatabase> m_pKeyFrameDatabase, std::shared_ptr<Map> m_pMap*/)
: m_imGrey()
, m_RGB()
, m_pSelfLocalization(pSelfLocalization)
{
  


  	m_RGB = (kv.getValue<int32_t>("logic-sensation-selflocalization.camera.channelorder.RGB") == 1);
	
}


Tracking::~Tracking()
{
}


}
}
}
