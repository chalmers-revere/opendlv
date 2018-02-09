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

#ifndef LOGIC_SENSATION_SELFLOCALIZATION_MAPPING_HPP
#define LOGIC_SENSATION_SELFLOCALIZATION_MAPPING_HPP

#include <memory>

#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include "opendavinci/odcore/base/Mutex.h"
#include <opendavinci/odcore/data/TimeStamp.h>
#include <opendavinci/odcore/strings/StringToolbox.h>
#include <opendavinci/odcore/wrapper/Eigen.h>
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"
#include "opendavinci/generated/odcore/data/CompactPointCloud.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
/*
#include "keyframe.hpp"
#include "map.hpp"
#include "loopclosing.hpp"
#include "tracking.hpp"
#include "keyframedatabase.hpp"
*/
namespace opendlv {
namespace logic {
namespace sensation {

class Mapping {
 public:
  Mapping(/*std::shared_ptr<Map> pMap,*/bool stereo);
	//void setTracker(std::shared_ptr<Tracking> pTracker);
	//void setLoopCloser(std::shared_ptr<LoopClosing> pLoopCloser);
	void Run();
/*Maybe not necessary*/	
  //Selflocalization(Selflocalization const &) = delete;
  //Selflocalization &operator=(Selflocalization const &) = delete;
  //virtual ~Selflocalization();
	

 private:

  bool m_bStereo;
	//std::shared_ptr<Map> m_pMap;
	//std::shared_ptr<Tracking> m_pTracker;
	//std::shared_ptr<LoopClosing> m_pLoopCloser;
	/*Variables needed to initialize threads and databases*/
  /*
	
	std::shared_ptr<OrbVocabulary> m_pVocabulary;
	std::shared_ptr<Map> m_pMap;
	std::shared_ptr<KeyFrameDatabase> m_pKeyFrameDatabase;
	std::shared_ptr<Tracking> m_pTracker;
	std::shared_ptr<Mapping> m_pMapper;
	std::shared_ptr<LoopClosing> m_pLoopCloser;

	std::shared_ptr<std::thread> m_pMappingThread;
	std::shared_ptr<std::thread> m_pLoopClosingThread;


*/

	
};

}
}
}

#endif
