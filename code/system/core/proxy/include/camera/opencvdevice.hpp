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

#ifndef CAMERA_OPENCVCAMERA_H_
#define CAMERA_OPENCVCAMERA_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "camera/device.hpp"

namespace opendlv {
namespace proxy {
namespace camera {

/**
 * This class wraps an OpenCV camera and captures its data into a shared memory
 * segment.
 */
class OpenCvDevice : public Device {
 public:
  OpenCvDevice(std::string const &, std::string const &, std::string const &, 
      std::string const &, uint32_t const &, uint32_t const &, 
      uint32_t const &);
  OpenCvDevice(OpenCvDevice const &) = delete;
  OpenCvDevice &operator=(OpenCvDevice const &) = delete;
  virtual ~OpenCvDevice();

 private:
  virtual bool CopyImageTo(char *, uint32_t const &);
  virtual bool IsValid() const;
  virtual bool CaptureFrame();

 private:
  std::unique_ptr<cv::VideoCapture> m_capture;
  cv::Mat m_image;
  cv::Mat m_distCoeff;
  cv::Mat m_cameraMatrix;
};

} // camera
} // proxy
} // opendlv

#endif
