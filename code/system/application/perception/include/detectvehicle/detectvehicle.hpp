/**
 * Copyright (C) 2017 Chalmers REVERE
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

#ifndef DETECTVEHICLE_DETECTVEHICLE_HPP_
#define DETECTVEHICLE_DETECTVEHICLE_HPP_

#include <vector>

#include <opencv2/objdetect/objdetect.hpp>

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

#include <Eigen/Dense>

namespace opendlv {
namespace perception {
namespace detectvehicle {

struct vehicle_t {
  int id;
  int x;
  int y;
  int w;
  int h;
  int found;  // AKA Rank
};

/**
 * This class provides a module with ability of detect vehicle.
 * It imports a trained ANN externally.
 */
class DetectVehicle
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  DetectVehicle(int32_t const &, char **);
  DetectVehicle(DetectVehicle const &) = delete;
  DetectVehicle &operator=(DetectVehicle const &) = delete;
  virtual ~DetectVehicle();
  virtual void nextContainer(odcore::data::Container &);



 private:
  void setUp();
  void tearDown();

  std::vector<vehicle_t> vehicle_buffer = std::vector<vehicle_t>();
  CvHaarClassifierCascade* cascade;
  CvMemStorage* storage;
  bool m_initialised;
  bool m_debug;

  /* --- CONFIGURATIONS (USER DEFINED) ---*/
  int input_resize_percent = 90;
  int NEIGHBOURS=3;
  int SHOW_POINT = 3; // MINUMUM RANK
  int REMOVE_POINT = -3;
  int SUCCESS_POINT=30; //
  int SHOW_RIO=0;
  int HORIZON=400;
  int id_counter = 0;
  int MAX_DISTANCE = 30;  // 30 pixels shift at most for each car from one frame to another



  /* --- METHODS ---*/
  void init();
  int checkMatch(vehicle_t vehicle);
  void drawRec(IplImage* img, vehicle_t vehicle);
  void detect(IplImage* img,odcore::data::TimeStamp timeStampOfFrame);
  double distance(int x1, int x2, int y1, int y2);
  void updateBuffer();
  void drawROI(IplImage* frame);
  void sendVehicle(vehicle_t vehicle,odcore::data::TimeStamp timeStampOfFrame);  // TODO


};

} // detectvehicle
} // perception
} // opendlv

#endif
