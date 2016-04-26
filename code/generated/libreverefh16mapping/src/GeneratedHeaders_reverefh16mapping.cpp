/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#include "generated/opendlv/proxy/reverefh16/Pedals.h"
#include "generated/opendlv/proxy/reverefh16/AccelerationRequest.h"
#include "generated/opendlv/proxy/reverefh16/BrakeRequest.h"
#include "generated/opendlv/proxy/reverefh16/SteeringRequest.h"
#include "generated/opendlv/proxy/reverefh16/Axles.h"
#include "generated/opendlv/proxy/reverefh16/Propulsion.h"
#include "generated/opendlv/proxy/reverefh16/VehicleState.h"
#include "generated/opendlv/proxy/reverefh16/Wheels.h"
#include "generated/opendlv/proxy/reverefh16/Steering.h"

#include "GeneratedHeaders_reverefh16mapping.h"

namespace canmapping {


    CanMapping::CanMapping() :
    m_pedals (),
    m_accelerationRequest (),
    m_brakeRequest (),
    m_steeringRequest (),
    m_axles (),
    m_propulsion (),
    m_vehicleState (),
    m_wheels (),
    m_steering ()
{}

    CanMapping::~CanMapping() {}

    vector<odcore::data::Container> CanMapping::mapNext(const ::automotive::GenericCANMessage &gcm) {
        vector<odcore::data::Container> listOfContainers;

        // Traverse all defined mappings and check whether a new high-level message could be fully decoded.
	    {
	    	odcore::data::Container container = m_pedals.decode(gcm);
	    	if (container.getDataType() != odcore::data::Container::UNDEFINEDDATA)
	    	{
	    		listOfContainers.push_back(container);
	    	}
	    }
	    {
	    	odcore::data::Container container = m_accelerationRequest.decode(gcm);
	    	if (container.getDataType() != odcore::data::Container::UNDEFINEDDATA)
	    	{
	    		listOfContainers.push_back(container);
	    	}
	    }
	    {
	    	odcore::data::Container container = m_brakeRequest.decode(gcm);
	    	if (container.getDataType() != odcore::data::Container::UNDEFINEDDATA)
	    	{
	    		listOfContainers.push_back(container);
	    	}
	    }
	    {
	    	odcore::data::Container container = m_steeringRequest.decode(gcm);
	    	if (container.getDataType() != odcore::data::Container::UNDEFINEDDATA)
	    	{
	    		listOfContainers.push_back(container);
	    	}
	    }
	    {
	    	odcore::data::Container container = m_axles.decode(gcm);
	    	if (container.getDataType() != odcore::data::Container::UNDEFINEDDATA)
	    	{
	    		listOfContainers.push_back(container);
	    	}
	    }
	    {
	    	odcore::data::Container container = m_propulsion.decode(gcm);
	    	if (container.getDataType() != odcore::data::Container::UNDEFINEDDATA)
	    	{
	    		listOfContainers.push_back(container);
	    	}
	    }
	    {
	    	odcore::data::Container container = m_vehicleState.decode(gcm);
	    	if (container.getDataType() != odcore::data::Container::UNDEFINEDDATA)
	    	{
	    		listOfContainers.push_back(container);
	    	}
	    }
	    {
	    	odcore::data::Container container = m_wheels.decode(gcm);
	    	if (container.getDataType() != odcore::data::Container::UNDEFINEDDATA)
	    	{
	    		listOfContainers.push_back(container);
	    	}
	    }
	    {
	    	odcore::data::Container container = m_steering.decode(gcm);
	    	if (container.getDataType() != odcore::data::Container::UNDEFINEDDATA)
	    	{
	    		listOfContainers.push_back(container);
	    	}
	    }

        return listOfContainers;
    }

} // canmapping
