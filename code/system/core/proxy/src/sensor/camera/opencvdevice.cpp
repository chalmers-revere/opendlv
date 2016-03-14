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

#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"

#include "sensor/camera/opencvdevice.hpp"

namespace opendlv {
namespace proxy {
namespace sensor {
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
OpenCvDevice::OpenCvDevice(std::string const &a_name, uint32_t const &a_id,
uint32_t const &a_width, uint32_t const &a_height, uint32_t const &a_bpp)
    : Device(a_name, a_id, a_width, a_height, a_bpp)
    , m_capture(nullptr)
    , m_image(nullptr)
{
  m_capture = cvCaptureFromCAM(a_id);

  if (m_capture) {
    cvSetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_WIDTH, a_width);
    cvSetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_HEIGHT, a_height);
  }
  else {
    std::cerr << "[proxy-sensor-camera] Could not open camera '" << a_name
              << "' with ID: " << a_id << std::endl;
  }
}

OpenCvDevice::~OpenCvDevice()
{
  if (m_capture) {
    cvReleaseCapture(&m_capture);
    m_capture = nullptr;
  }
}

bool OpenCvDevice::IsValid() const
{
  return (m_capture != nullptr);
}

bool OpenCvDevice::CaptureFrame()
{
  bool retVal = false;
  if (m_capture != nullptr) {
    if (cvGrabFrame(m_capture)) {
      if (GetBpp() == 1) {
        IplImage *tmpFrame = cvRetrieveFrame(m_capture);

        if (m_image == nullptr) {
          m_image = cvCreateImage(cvGetSize(tmpFrame), IPL_DEPTH_8U, 1);
        }

        cvCvtColor(tmpFrame, m_image, CV_BGR2GRAY);
      }
      else {
        m_image = cvRetrieveFrame(m_capture);
      }

      retVal = true;
    }
  }
  return retVal;
}

bool OpenCvDevice::CopyImageTo(char *a_destination, const uint32_t &a_size)
{
  bool retVal = false;

  if ((a_destination != nullptr) && (a_size > 0) && (m_image != nullptr)) {
    ::memcpy(a_destination, m_image->imageData, a_size);

    cvShowImage("WindowShowImage", m_image);
    cvWaitKey(10);

    retVal = true;
  }

  return retVal;
}

} // camera
} // sensor
} // proxy
} // opendlv
