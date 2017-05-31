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

#ifndef LOGIC_PERCEPTION_DETECTVEHICLE_TRACKVEHICLE_HPP
#define LOGIC_PERCEPTION_DETECTVEHICLE_TRACKVEHICLE_HPP

#include <memory>
#include <string>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

namespace opendlv {
namespace logic {
namespace perception {



  struct vehicle_t {
    int id;
    int x;
    int y;
    int w;
    int h;
    int found;  // AKA Rank
  };

  struct vehicle_tex {
    vehicle_tex()
    : vehicle()
    , image() {}
    vehicle_t vehicle;
    cv::Mat image;
  };
/**
 * This class provides a module with capability of keeping track of vehicle in a given series of images
 */
class TrackVehicle
: public odcore::base::module::DataTriggeredConferenceClientModule {
  

 public:
  TrackVehicle(int32_t const &, char **);
  TrackVehicle(TrackVehicle const &) = delete;
  TrackVehicle &operator=(TrackVehicle const &) = delete;
  virtual ~TrackVehicle();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();

  bool m_initialised;
  bool m_debug;

  std::vector<vehicle_tex> vehicle_buffer = std::vector<vehicle_tex>();
  std::vector<vehicle_t> vehicle_buffer_t = std::vector<vehicle_t>();

  void pushToBuffer(vehicle_tex data);
  void updateBuffer(vehicle_t data);
  void doTrack(cv::Mat frame);
  cv::Mat getVehicleFromFrame(cv::Mat frame, vehicle_t data);
  cv::Mat matchFeatures(cv::Mat img_object, cv::Mat img_scene);
//  void doTrack(cv::Mat frame);

};

} // trackvehicle
} // perception
} // opendlv

#endif
