/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_DMCP_CONSTANTS_H
#define COREDATA_DMCP_CONSTANTS_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace coredata {
	namespace dmcp {
		using namespace std;
		
		class Constants : public core::data::SerializableData, public core::base::Visitable {
			public:
				enum DMCP {
					DISCOVER_TIMEOUT = 1000,
					BROADCAST_PORT_CLIENT = 19751,
					CONNECTION_TIMEOUT = 1000,
					CONNECTIONSERVER_PORT_BASE = 19755,
					CONNECTION_RETRIES = 5,
					CONFIGURATION_TIMEOUT = 1000,
					BROADCAST_PORT_SERVER = 19750,
				};
			public:
				Constants();
		
		
				virtual ~Constants();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				Constants(const Constants &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				Constants& operator=(const Constants &obj);
		
			public:
		
			public:
				/**
				 * This method returns the message id.
				 *
				 * @return Message id.
				 */
				static int32_t ID();
		
				/**
				 * This method returns the short message name.
				 *
				 * @return Short message name.
				 */
				static const string ShortName();
		
				/**
				 * This method returns the long message name include package/sub structure.
				 *
				 * @return Long message name.
				 */
				static const string LongName();
		
			public:
				virtual void accept(core::base::Visitor &v);
		
				virtual ostream& operator<<(ostream &out) const;
				virtual istream& operator>>(istream &in);
		
				virtual const string toString() const;
		
			private:
		
		};
	} // dmcp
} // coredata

#endif /*COREDATA_DMCP_CONSTANTS_H*/
