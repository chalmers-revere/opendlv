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

#include <iostream>

#include "opendavinci/odcore/base/Thread.h"

#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"

#include "camera/opencvdevice.hpp"

namespace opendlv {
namespace proxy {
namespace camera {

/**
 * Constructor.
 *
 * @param name Name of the shared memory segment.
 * @param id OpenCVDevice identifier.
 * @param width
 * @param height
 * @param bpp
 */
OpenCvDevice::OpenCvDevice(std::string const &a_name, 
    std::string const &a_port, std::string const &a_username, 
    std::string const &a_password, uint32_t const &a_width, 
    uint32_t const &a_height, uint32_t const &a_bpp)
    : Device(a_name, a_width, a_height, a_bpp)
    , m_capture(nullptr)
    , m_image(new cv::Mat)
{
  std::string videoStreamAddress = std::string("http://") + a_username 
    + ":" + a_password + "@" + a_port + "/axis-cgi/mjpg/video.cgi?user=" 
    + a_username + "&password=" + a_password + "&channel=0&.mjpg";


  std::cout << videoStreamAddress << std::endl;


//  videoStreamAddress = "rostock.avi";

  m_capture.reset(new cv::VideoCapture(videoStreamAddress));

  //odcore::base::Thread::usleepFor(2000);

  if (m_capture->isOpened()) {
    std::cout << "Open. width: " << a_width << " height: " << a_height << std::endl;
    m_capture->set(CV_CAP_PROP_FRAME_WIDTH, a_width);
    m_capture->set(CV_CAP_PROP_FRAME_HEIGHT, a_height);
  }
  else {
    std::cerr << "[proxy-camera] Could not open camera '" << a_name
              << std::endl;
  }
}

OpenCvDevice::~OpenCvDevice()
{
  if (m_capture) {
    m_capture->release();
    m_capture = nullptr;
  }
}

bool OpenCvDevice::IsValid() const
{
  return m_capture->isOpened();
}

bool OpenCvDevice::CaptureFrame()
{
  bool retVal = false;
  if (m_capture != nullptr) {
    if (m_capture->read(*m_image)) {
      retVal = true;
    }
  }
  return retVal;
}

bool OpenCvDevice::CopyImageTo(char *a_destination, const uint32_t &a_size)
{
  bool retVal = false;

  if ((a_destination != nullptr) && (a_size > 0) && (m_image != nullptr)) {
    ::memcpy(a_destination, m_image->data, a_size);

    cv::imshow("Camera feed", *m_image);
    cv::waitKey(10);

    retVal = true;
  }

  return retVal;
}

} // camera
} // proxy
} // opendlv
