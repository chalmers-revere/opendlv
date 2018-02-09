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

#include"mapping.hpp"
#include "opendavinci/odcore/data/TimeStamp.h"

namespace opendlv {
namespace logic {
namespace sensation {

Mapping::Mapping(/*std::shared_ptr<Map> pMap,*/bool stereo):
	m_bStereo(Stereo)
	//,m_pMap(pMap)
{
	std::cout << "Mapping Calling, Stereo: " << m_bStereo << std::endl;
}
	/*
void Mapping::setLoopCloser(std::shared_ptr<LoopClosing> pLoopCloser){
	m_pLoopCloser = pLoopCLoser;
}

void Mapping::setTracker(std::shared_ptr<Tracking> pTracker){
	m_pTracker = pTracker;
}
*/
}
}
}
