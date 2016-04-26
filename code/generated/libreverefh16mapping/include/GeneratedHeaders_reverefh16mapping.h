/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef GENERATEDHEADERS_REVEREFH16MAPPING_H_
#define GENERATEDHEADERS_REVEREFH16MAPPING_H_

#include <string>
#include <vector>

#include "generated/opendlv/proxy/reverefh16/Pedals.h"
#include "generated/opendlv/proxy/reverefh16/AccelerationRequest.h"
#include "generated/opendlv/proxy/reverefh16/BrakeRequest.h"
#include "generated/opendlv/proxy/reverefh16/SteeringRequest.h"
#include "generated/opendlv/proxy/reverefh16/Axles.h"
#include "generated/opendlv/proxy/reverefh16/Propulsion.h"
#include "generated/opendlv/proxy/reverefh16/VehicleState.h"
#include "generated/opendlv/proxy/reverefh16/Wheels.h"
#include "generated/opendlv/proxy/reverefh16/Steering.h"

#include <opendavinci/odcore/data/Container.h>

#include <automotivedata/GeneratedHeaders_AutomotiveData.h>
#include <opendlvdata/GeneratedHeaders_opendlvdata.h>

namespace canmapping {

    using namespace std;

    class CanMapping {
        private:
            /**
             * "Forbidden" copy constructor. Goal: The compiler should warn
             * already at compile time for unwanted bugs caused by any misuse
             * of the copy constructor.
             *
             * @param obj Reference to an object of this class.
             */
            CanMapping(const CanMapping &/*obj*/);

            /**
             * "Forbidden" assignment operator. Goal: The compiler should warn
             * already at compile time for unwanted bugs caused by any misuse
             * of the assignment operator.
             *
             * @param obj Reference to an object of this class.
             * @return Reference to this instance.
             */
            CanMapping& operator=(const CanMapping &/*obj*/);

        public:
            CanMapping();

            virtual ~CanMapping();

            /**
             * This method adds the given GenericCANMessage to the internal
             * CAN message decoder. If this message could be decoded (or
             * including the previous sequence, this method returns a valid
             * Container (ie. Container::UNDEFINEDDATA).
             *
             * @param gcm Next GenericCANMessage.
             * @return Container, where the type needs to be checked to determine invalidity (i.e. !Container::UNDEFINEDDATA).
             */
            vector<odcore::data::Container> mapNext(const ::automotive::GenericCANMessage &gcm);

        private:
        
				opendlv::proxy::reverefh16::Pedals m_pedals;
				opendlv::proxy::reverefh16::AccelerationRequest m_accelerationRequest;
				opendlv::proxy::reverefh16::BrakeRequest m_brakeRequest;
				opendlv::proxy::reverefh16::SteeringRequest m_steeringRequest;
				opendlv::proxy::reverefh16::Axles m_axles;
				opendlv::proxy::reverefh16::Propulsion m_propulsion;
				opendlv::proxy::reverefh16::VehicleState m_vehicleState;
				opendlv::proxy::reverefh16::Wheels m_wheels;
				opendlv::proxy::reverefh16::Steering m_steering;
		
    };

} // canmapping

#endif /*GENERATEDHEADERS_REVEREFH16MAPPING_H_*/
