/**
 * Copyright (C) 2016 Chalmers REVERE
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
#include <string>
#include <vector>

#if !defined(WIN32) && !defined(__gnu_hurd__) && !defined(__APPLE__)
#include <cstdlib>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#endif

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <odcantools/MessageToCANDataStore.h>
#include <odcantools/CANDevice.h>
#include <automotivedata/generated/automotive/GenericCANMessage.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "canmessagedatastore.hpp"
#include "ps3controller/ps3controller.hpp"

namespace opendlv {
namespace tools {
namespace can {
namespace ps3controller {

PS3Controller::PS3Controller(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(a_argc, a_argv, "tools-can-ps3controller")
    , automotive::odcantools::GenericCANMessageListener()
    , m_fifo()
    , m_device()
    , m_canMessageDataStore()
    , m_revereFh16CanMessageMapping()
    , m_ps3controllerFD(0)
    , m_axes(0)
    , m_incrementRotation(0)
    , m_incrementAcceleration(0)
{
}

PS3Controller::~PS3Controller()
{
}

void PS3Controller::setUp()
{
  using namespace std;
  using namespace odcore::base;
  using namespace odcore::data;
  using namespace odtools::recorder;
  using namespace automotive::odcantools;

  string const PS3CONTROLLER_DEVICE_NODE =
  getKeyValueConfiguration().getValue<string>("tools-can-ps3controller.ps3controllerdevicenode");

  string const DEVICE_NODE =
  getKeyValueConfiguration().getValue<string>("tools-can-ps3controller.devicenode");

  // Try to open CAN device and register this instance as receiver for
  // GenericCANMessages.
  m_device = shared_ptr<CANDevice>(new CANDevice(DEVICE_NODE, *this));

  // If the device could be successfully opened, create a recording file with a
  // dump of the data.
  if (m_device.get() && m_device->isOpen()) {
    cout << "Successfully opened CAN device '" << DEVICE_NODE << "'." << endl;

    // Start the wrapped CAN device to receive CAN messages concurrently.
    m_device->start();
  }

  // Setup ps3controller control.
  {
    cout << "Trying to open ps3controller " << PS3CONTROLLER_DEVICE_NODE << endl;
#if !defined(WIN32) && !defined(__gnu_hurd__) && !defined(__APPLE__)
    int num_of_axes = 0;
    int num_of_buttons = 0;

    int name_of_ps3controller[80];

    if ((m_ps3controllerFD = open(PS3CONTROLLER_DEVICE_NODE.c_str(), O_RDONLY)) == -1) {
      cerr << "Could not open ps3controller " << PS3CONTROLLER_DEVICE_NODE << endl;
      return;
    }

    ioctl(m_ps3controllerFD, JSIOCGAXES, &num_of_axes);
    ioctl(m_ps3controllerFD, JSIOCGBUTTONS, &num_of_buttons);
    ioctl(m_ps3controllerFD, JSIOCGNAME(80), &name_of_ps3controller);

    m_axes = (int *)calloc(num_of_axes, sizeof(int));
    cerr << "PS3Controller found " << name_of_ps3controller
         << ", number of axes: " << num_of_axes
         << ", number of buttons: " << num_of_buttons << endl;

    // Use non blocking reading.
    fcntl(m_ps3controllerFD, F_SETFL, O_NONBLOCK);
#else
    cerr << "This code will not work on this computer architecture " << endl;
#endif
  }
}

void PS3Controller::tearDown()
{
  if (m_device.get()) {
    // send false packets
    
    automotive::GenericCANMessage gcm;
    
    opendlv::proxy::reverefh16::BrakeRequest brakeRequest;
    brakeRequest.setEnableRequest(false);
    // Create the message mapping.
    canmapping::opendlv::proxy::reverefh16::BrakeRequest brakeRequestMapping;
    // The high-level message needs to be put into a Container.
    odcore::data::Container br(brakeRequest);
    gcm = brakeRequestMapping.encode(br);
    m_device->write(gcm);
    
    opendlv::proxy::reverefh16::AccelerationRequest accelerationRequest;
    accelerationRequest.setEnableRequest(false);
    // Create the message mapping.
    canmapping::opendlv::proxy::reverefh16::AccelerationRequest accelerationRequestMapping;
    // The high-level message needs to be put into a Container.
    odcore::data::Container ar(accelerationRequest);
    gcm = accelerationRequestMapping.encode(ar);
    m_device->write(gcm);
    
    opendlv::proxy::reverefh16::SteeringRequest steeringRequest;
    steeringRequest.setEnableRequest(false);
    // Create the message mapping.
    canmapping::opendlv::proxy::reverefh16::SteeringRequest steeringRequestMapping;
    // The high-level message needs to be put into a Container.
    odcore::data::Container sr(steeringRequest);
    gcm = steeringRequestMapping.encode(sr);
    m_device->write(gcm);
    
    // Stop the wrapper CAN device.
    m_device->stop();
  }

  // Deactivate ps3controller control.
  {
    close(m_ps3controllerFD);
  }
}

void PS3Controller::nextGenericCANMessage(const automotive::GenericCANMessage &gcm)
{
  // Map CAN message to high-level data structure.
  vector<odcore::data::Container> result = m_revereFh16CanMessageMapping.mapNext(gcm);

  if (result.size() > 0) {
    auto it = result.begin();
    while (it != result.end()) {
      odcore::data::Container c = (*it);
      // Send container to conference.
      getConference().send(c);
      it++;
    }
  }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode PS3Controller::body()
{
  /* Lessons learned from live test:
   * - Activating the program takes exclusive control of acceleration 
   * (no manual override, unless the Emergency switch is pressed)
   * - Care must be taken when activating the brakes, since braking too much 
   * is *very* uncomfortable -and potentially dangerous-, on the other hand a 
   * smaller maximum value for the deceleration could be dangerous as well, since it
   * could impair the possiblity to perform an emergency brake while using the controller.
   */
  const double RANGE_ACCELERATION_MIN = 0; // acceleration can be between 0%...
  const double RANGE_ACCELERATION_MAX = 50; // ...and 50% 
  const double RANGE_DECELERATION_MIN = 0; // deceleration can be between 0 m/s^2...
  const double RANGE_DECELERATION_MAX = -10; // ...and -10 m/s^2
  const double RANGE_ROTATION_MIN = -10; // the torque can be between -10 Nm...
  const double RANGE_ROTATION_MAX = 10; // ...and 10 Nm
  
  double acceleration = 0;
  double deceleration = 0;
  double rotation = 0;
  double percent=0.0;
  
  automotive::GenericCANMessage gcm;

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

#if !defined(WIN32) && !defined(__gnu_hurd__) && !defined(__APPLE__)
    struct js_event js;
    
    while (read(m_ps3controllerFD, &js, sizeof(struct js_event)) > 0) 
    {
        percent=0.0;
        // Check event.
        switch (js.type & ~JS_EVENT_INIT) {
            case JS_EVENT_AXIS:
            
                m_axes[js.number] = js.value;
                if(js.number==0){ // LEFT ANALOG STICK
                    CLOG2 << "Axis number " << (int)js.number << " with value " << (int)js.value;
                    // this will return a percent value over the whole range
                    percent=(double)(js.value-MIN_AXES_VALUE)/(double)(MAX_AXES_VALUE-MIN_AXES_VALUE)*100;
                    if(percent>49.95 && percent<50.05) {
                        CLOG2<<" : going straight "<<endl;
                    } else {
                        // this will return values in the range [0-100] for both a left or right turn (instead of [0-50] for left and [50-100] for right)
                        CLOG2<<" : turning "<< (js.value<0?"left":"right") <<" at "<< (js.value<0?(100.0-2*percent):(2*percent-100.0)) <<"%"<<endl;
                    }
                    
                    // map the rotation from percentage to its range
                    rotation=percent/100*(RANGE_ROTATION_MAX-RANGE_ROTATION_MIN)+RANGE_ROTATION_MIN;
                    // modify in steps of 0.5
                    rotation=round(2*rotation)/2;
                }
                
                // no else-if as many of these events can occur simultaneously
                if(js.number==3){ // RIGHT ANALOG STICK
                    CLOG2 << "Axis number " << (int)js.number << " with value " << (int)js.value;
                    // this will return a percent value over the whole range
                    percent=(double)(js.value-MIN_AXES_VALUE)/(double)(MAX_AXES_VALUE-MIN_AXES_VALUE)*100;
                    // this will return values in the range [0-100] for both accelerating and braking (instead of [50-0] for accelerating and [50-100] for braking)
                    CLOG2<<" : "<< (js.value<0?"accelerating":"braking") <<" at "<< (js.value<0?(100.0-2*percent):(2*percent-100.0)) <<"%"<<endl;
                    
                    if(js.value<0) {
                        // map the acceleration from percentage to its range
                        acceleration=(100.0-2*percent)/100*(RANGE_ACCELERATION_MAX-RANGE_ACCELERATION_MIN)+RANGE_ACCELERATION_MIN;
                    }
                    else {
                        // map the deceleration from percentage to its range
                        deceleration=(2*percent-100.0)/100*(RANGE_DECELERATION_MAX-RANGE_DECELERATION_MIN);
                    }
                    
                    // modify in steps of 0.5
                    acceleration=round(2*acceleration)/2;
                    deceleration=round(2*deceleration)/2;
                    
                    // to avoid showing "-0" (just "0" looks better imo)
                    if(deceleration<0.01 && deceleration>-0.01) deceleration=0.0;
                }
                
                // no else-if as many of these events can occur simultaneously
                if(js.number==18){ // X BUTTON AXIS
                    CLOG2 << "Axis number " << (int)js.number << " with value " << (int)js.value << " (X BUTTON AXIS)"<<endl;
                    // sends a false flagged braking packet
                    {
                        opendlv::proxy::reverefh16::BrakeRequest brakeRequest;
                        brakeRequest.setEnableRequest(false);
                        canmapping::opendlv::proxy::reverefh16::BrakeRequest brakeRequestMapping;
                        odcore::data::Container br(brakeRequest);
                        gcm = brakeRequestMapping.encode(br);
                        m_device->write(gcm);
                    }
                }
                // no else-if as many of these events can occur simultaneously
                if(js.number==17){ // CIRCLE BUTTON AXIS
                    CLOG2 << "Axis number " << (int)js.number << " with value " << (int)js.value << " (CIRCLE BUTTON AXIS)"<<endl;
                    
                    // sends a false flagged steering packet
                    {
                        opendlv::proxy::reverefh16::SteeringRequest steeringRequest;
                        steeringRequest.setEnableRequest(false);
                        canmapping::opendlv::proxy::reverefh16::SteeringRequest steeringRequestMapping;
                        odcore::data::Container sr(steeringRequest);
                        gcm = steeringRequestMapping.encode(sr);
                        m_device->write(gcm);
                    }
                }
                // no else-if as many of these events can occur simultaneously
                if(js.number==19){ // SQUARE BUTTON AXIS
                    CLOG2 << "Axis number " << (int)js.number << " with value " << (int)js.value << " (SQUARE BUTTON AXIS)"<<endl;
                    // kills the acceleration
                    acceleration=0.0;
                    deceleration=0.0;
                }
                // no else-if as many of these events can occur simultaneously
                if(js.number==16){ // TRIANGLE BUTTON AXIS
                    CLOG2 << "Axis number " << (int)js.number << " with value " << (int)js.value << " (TRIANGLE BUTTON AXIS)"<<endl;
                    // kills the rotation
                    rotation=0.0;
                }
            break;
            
            case JS_EVENT_BUTTON: break;
            
            case JS_EVENT_INIT: break;
            
            default: break;
        }
   	}
   	
   	/* EAGAIN is returned when the queue is empty */
   	if (errno != EAGAIN) {
        cerr << "An error occurred in the PS3 joystick event handling" << endl;
        return odcore::data::dmcp::ModuleExitCodeMessage::SERIOUS_ERROR;
   	}
   	
#else
    if (m_axes!=NULL && m_axes[1]!=NULL) {
        if(m_axes[1] > 0){
            // Braking.
            deceleration = (m_axes[1]/MAX)*FACTOR_ACCELERATION;
            //m_behaviour.brake(deceleration);
        }
        else {
            // Accelerating.
            acceleration = ((-m_axes[1])/MAX)*FACTOR_ACCELERATION;
            //m_behaviour.accelerate(acceleration);
        }

        // Steering.
        rotation = (m_axes[0]/MAX)*FACTOR_ROTATION;
    }
#endif

    cout << "Values: A: " << acceleration << ", B: " << deceleration << ", R: " << rotation << endl;
    
    {
        opendlv::proxy::reverefh16::BrakeRequest brakeRequest;
        brakeRequest.setEnableRequest(true);
        brakeRequest.setBrake(deceleration);
        // Create the message mapping.
        canmapping::opendlv::proxy::reverefh16::BrakeRequest brakeRequestMapping;
        // The high-level message needs to be put into a Container.
        odcore::data::Container br(brakeRequest);
        gcm = brakeRequestMapping.encode(br);
        m_device->write(gcm);
        
        opendlv::proxy::reverefh16::AccelerationRequest accelerationRequest;
        accelerationRequest.setEnableRequest(true);
        accelerationRequest.setAccelerationPedalPosition(acceleration);
        // Create the message mapping.
        canmapping::opendlv::proxy::reverefh16::AccelerationRequest accelerationRequestMapping;
        // The high-level message needs to be put into a Container.
        odcore::data::Container ar(accelerationRequest);
        gcm = accelerationRequestMapping.encode(ar);
        m_device->write(gcm);
        
        opendlv::proxy::reverefh16::SteeringRequest steeringRequest;
        steeringRequest.setEnableRequest(true);
        steeringRequest.setSteeringRoadWheelAngle(0);
        steeringRequest.setSteeringDeltaTorque(rotation);
        // Create the message mapping.
        canmapping::opendlv::proxy::reverefh16::SteeringRequest steeringRequestMapping;
        // The high-level message needs to be put into a Container.
        odcore::data::Container sr(steeringRequest);
        gcm = steeringRequestMapping.encode(sr);
        m_device->write(gcm);
    }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

} // ps3controller
} // can
} // tools
} // opendlv
