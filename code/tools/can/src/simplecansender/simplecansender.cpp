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

#include <cctype>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <odcantools/MessageToCANDataStore.h>
#include <odcantools/CANDevice.h>
#include <automotivedata/generated/automotive/GenericCANMessage.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "reverefh16mapping/GeneratedHeaders_reverefh16mapping.h"

#include "canmessagedatastore.hpp"
#include "simplecansender/simplecansender.hpp"

namespace opendlv {
    namespace tools {
        namespace can {
            namespace simplecansender {

                using namespace std;
                using namespace odcore::base;
                using namespace odcore::data;
                using namespace odtools::recorder;
                using namespace automotive::odcantools;

                SimpleCANSender::SimpleCANSender(int32_t const &a_argc, char **a_argv)
                    : odcore::base::module::TimeTriggeredConferenceClientModule(
                      a_argc, a_argv, "tools-can-simplecansender")
                    , automotive::odcantools::GenericCANMessageListener()
                    , m_fifo()
                    , m_device()
                    , m_canMessageDataStore()
                    , m_revereFh16CanMessageMapping()
                    , m_lastMessage()
                {
                }

                SimpleCANSender::~SimpleCANSender()
                {
                }

                void SimpleCANSender::setUp()
                {
                    string const DEVICE_NODE =
                    getKeyValueConfiguration().getValue<string>("tools-can-simplecansender.devicenode");

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
                    cout << endl << "Simple CAN request sender application" << endl 
                                << "\tStructure for messages:" << endl
                                << "\tAcceleration Request:  ACR:Accel_request_Value:Enable_Value:" << endl
                                << "\tBrake Request:         BRR:Brake_request_Value:Enable_Value:" << endl
                                << "\tSteering Request:      STR:Delta_trq:RWA_req_Value:Enable_Value:" << endl 
                                << "Example: \n\tThe string 'BRR:-10:1:' will send an enabled brake request of -10m/s^2" << endl 
                                << "Special strings: \n\t'r:' will repeat the last message \n\t'exit:' will close the application" << endl << endl ;
                }

                void SimpleCANSender::tearDown()
                {
                    if (m_device.get()) {
                        // disable the enable bits
                        disableCmds();
                        // Stop the wrapper CAN device.
                        m_device->stop();
                    }
                }

                void SimpleCANSender::nextGenericCANMessage(const automotive::GenericCANMessage &gcm)
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

                void SimpleCANSender::disableCmds()
                {
                    if (m_device.get()) {
                        CLOG1<<" Clearing Enable bits..."<<endl;
                        // Disabling steering request at shutdown.
                        {
                            opendlv::proxy::reverefh16::SteeringRequest steerRequest;
                            steerRequest.setEnableRequest(false);
                            steerRequest.setSteeringRoadWheelAngle(0);
                            steerRequest.setSteeringDeltaTorque(33.535);

                            // Create the message mapping.
                            canmapping::opendlv::proxy::reverefh16::SteeringRequest steeringRequestMapping;
                            // The high-level message needs to be put into a Container.
                            odcore::data::Container c(steerRequest);
                            automotive::GenericCANMessage gcm = steeringRequestMapping.encode(c);
                            m_device->write(gcm);
                        }
                        // Disabling acceleration request at shutdown.
                        {
                            opendlv::proxy::reverefh16::AccelerationRequest accelerationRequest;
                            accelerationRequest.setEnableRequest(false);
                            accelerationRequest.setAcceleration(0);

                            // Create the message mapping.
                            canmapping::opendlv::proxy::reverefh16::AccelerationRequest accelerationRequestMapping;
                            // The high-level message needs to be put into a Container.
                            odcore::data::Container c(accelerationRequest);
                            automotive::GenericCANMessage gcm = accelerationRequestMapping.encode(c);
                            m_device->write(gcm);
                        }
                        // Disabling brake request at shutdown.
                        {
                            opendlv::proxy::reverefh16::BrakeRequest brakeRequest;
                            brakeRequest.setEnableRequest(false);
                            brakeRequest.setBrake(0);

                            // Create the message mapping.
                            canmapping::opendlv::proxy::reverefh16::BrakeRequest brakeRequestMapping;
                            // The high-level message needs to be put into a Container.
                            odcore::data::Container c(brakeRequest);
                            automotive::GenericCANMessage gcm = brakeRequestMapping.encode(c);
                            m_device->write(gcm);
                        }
                    }
                }

                odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SimpleCANSender::body()
                {
                    const uint32_t MAX_MESSAGES = 500;
                    const double tolerance=0.001;
                    string token;
                    const float REQUIRED_FREQ = 100.0;
                    if (fabs(getFrequency() - REQUIRED_FREQ) > 1e-5f) 
                    {
                        cerr << getName() << " was not started with --freq=" << REQUIRED_FREQ << "! Aborting..." << endl;
                        return odcore::data::dmcp::ModuleExitCodeMessage::SERIOUS_ERROR;
                    }
                    
                    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) 
                    {
                        cout << ">> ";
                        
                        cin.clear();
                        getline(cin,token,':');
                        
                        CLOG1<<"First token: "<<token<<" (length "<<token.length()<<")"<<endl;
                        
                        if(token=="ACR" || (token.length()==4 && token.substr(0,3)=="ACR" && isspace(token.at(3))))
                        {
                            opendlv::proxy::reverefh16::AccelerationRequest accelerationRequest;
                            
                            getline(cin,token,':');
                            double acceleration=stod(token);
                            CLOG1<<"Acceleration value: "<<acceleration<<endl;
                            accelerationRequest.setAcceleration(acceleration);
                            
                            getline(cin,token,':');
                            double enable=stod(token);
                            if(enable>1-tolerance && enable<1+tolerance)
                            {
                                CLOG1<<"Enable bit is true"<<endl;
                                accelerationRequest.setEnableRequest(true);
                            }
                            else
                            {
                                CLOG1<<"Enable bit is false"<<endl;
                                accelerationRequest.setEnableRequest(false);
                            }
                            
                            // Create the message mapping.
                            canmapping::opendlv::proxy::reverefh16::AccelerationRequest accelerationRequestMapping;
                            // The high-level message needs to be put into a Container.
                            odcore::data::Container c(accelerationRequest);
                            m_lastMessage = accelerationRequestMapping.encode(c);

                            clog << "Sending " << MAX_MESSAGES << " messages...";
                            uint32_t cnt = MAX_MESSAGES;
                            while ( (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) && (--cnt > 0) ) {
                                m_device->write(m_lastMessage);
                            }
                            clog << " done." << endl;
                        }
                        else if(token=="BRR" || (token.length()==4 && token.substr(0,3)=="BRR" && isspace(token.at(3))))
                        {
                            opendlv::proxy::reverefh16::BrakeRequest brakeRequest;
                            
                            getline(cin,token,':');
                            double brake=stod(token);
                            CLOG1<<"Brake value: "<<brake<<endl;
                            brakeRequest.setBrake(brake);
                            
                            getline(cin,token,':');
                            double enable=stod(token);
                            if(enable>1-tolerance && enable<1+tolerance)
                            {
                                CLOG1<<"Enable bit is true"<<endl;
                                brakeRequest.setEnableRequest(true);
                            }
                            else
                            {
                                CLOG1<<"Enable bit is false"<<endl;
                                brakeRequest.setEnableRequest(false);
                            }
                            
                            // Create the message mapping.
                            canmapping::opendlv::proxy::reverefh16::BrakeRequest brakeRequestMapping;
                            // The high-level message needs to be put into a Container.
                            odcore::data::Container c(brakeRequest);
                            m_lastMessage = brakeRequestMapping.encode(c);

                            clog << "Sending " << MAX_MESSAGES << " messages...";
                            uint32_t cnt = MAX_MESSAGES;
                            while ( (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) && (--cnt > 0) ) {
                                m_device->write(m_lastMessage);
                            }
                            clog << " done." << endl;
                        }
                        else if(token=="STR" || (token.length()==4 && token.substr(0,3)=="STR" && isspace(token.at(3))))
                        {
                            opendlv::proxy::reverefh16::SteeringRequest steeringRequest;
                            
                            getline(cin,token,':');
                            double delta=stod(token);
                            CLOG1<<"Delta Torque value: "<<delta<<endl;
                            steeringRequest.setSteeringDeltaTorque(delta);
                            
                            getline(cin,token,':');
                            double rwa=stod(token);
                            CLOG1<<"Road Wheel Angle value: "<<rwa<<endl;
                            steeringRequest.setSteeringRoadWheelAngle(rwa);
                            
                            getline(cin,token,':');
                            double enable=stod(token);
                            if(enable>1-tolerance && enable<1+tolerance)
                            {
                                CLOG1<<"Enable bit is true"<<endl;
                                steeringRequest.setEnableRequest(true);
                            }
                            else
                            {
                                CLOG1<<"Enable bit is false"<<endl;
                                steeringRequest.setEnableRequest(false);
                            }
                            
                            // Create the message mapping.
                            canmapping::opendlv::proxy::reverefh16::SteeringRequest steeringRequestMapping;
                            // The high-level message needs to be put into a Container.
                            odcore::data::Container c(steeringRequest);
                            m_lastMessage = steeringRequestMapping.encode(c);

                            clog << "Sending " << MAX_MESSAGES << " messages...";
                            uint32_t cnt = MAX_MESSAGES;
                            while ( (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) && (--cnt > 0) ) {
                                m_device->write(m_lastMessage);
                            }
                            clog << " done." << endl;
                        }
                        else if(token=="r" || (token.at(0)=='r' && isspace(token.at(1))))
                        {
                            clog << "Re-sending last messages " << MAX_MESSAGES << " times...";
                            uint32_t cnt = MAX_MESSAGES;
                            while ( (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) && (--cnt > 0) ) {
                                m_device->write(m_lastMessage);
                            }
                            clog << " done." << endl;
                        }
                        else if(token=="exit")
                        {
                            clog <<"Closing application..." << endl;
                            return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
                        }
                        else
                        {
                            cerr<<"Character sequence not recognized. Try again."<<endl;
                        }
                        cin.ignore();
                    }
                    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
                }

            } // simplecansender
        } // can
    } // tools
} // opendlv
