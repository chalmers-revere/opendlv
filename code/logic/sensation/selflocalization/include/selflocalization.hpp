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

#ifndef LOGIC_SENSATION_SELFLOCALIZATION_SELFLOCALIZATION_HPP
#define LOGIC_SENSATION_SELFLOCALIZATION_SELFLOCALIZATION_HPP


#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <opendavinci/odcore/strings/StringToolbox.h>
#include <opendavinci/odcore/wrapper/Eigen.h>
#include <opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/data/Container.h>
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"
#include "opendavinci/generated/odcore/data/CompactPointCloud.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "opendavinci/odcore/base/KeyValueConfiguration.h"

#include "tracking.hpp"
#include "mapping.hpp"
#include "imageextractor.hpp"
<<<<<<< HEAD
#include "orbextractor.hpp"
=======
#include "orbvocabulary.hpp"
>>>>>>> ea5f74670cc9e5cfc06fc826e7e2a1277e0ef682

namespace opendlv {
namespace logic {
namespace sensation {	

class Selflocalization
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  Selflocalization(int32_t const &, char **);
  Selflocalization(Selflocalization const &) = delete;
  Selflocalization &operator=(Selflocalization const &) = delete;
  virtual ~Selflocalization();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();
  bool m_cameraType;
  int m_saveCounter = 0;
  std::shared_ptr<Tracking> m_pTracker;
  std::shared_ptr<Mapping> m_pMapper;
<<<<<<< HEAD
  std::shared_ptr<ImageExtractor> m_pImageGrab;
  std::shared_ptr<OrbExtractor> m_pExtractOrb;	
	/*Variables needed to initialize threads and databases*/
  /*
	
	std::shared_ptr<OrbVocabulary> m_pVocabulary;
	std::shared_ptr<Map> m_pMap;
=======
  std::shared_ptr<ImageExtractor> m_pImageGrab;	
 /*Variables needed to initialize threads and databases*/
 
  std::shared_ptr<OrbVocabulary> m_pVocabulary;
	/*std::shared_ptr<Map> m_pMap;
>>>>>>> ea5f74670cc9e5cfc06fc826e7e2a1277e0ef682
	std::shared_ptr<KeyFrameDatabase> m_pKeyFrameDatabase;
	
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
