/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_DMCP_DISCOVERMESSAGE_H
#define COREDATA_DMCP_DISCOVERMESSAGE_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"

#include "generated/coredata/dmcp/ServerInformation.h"

namespace coredata {
	namespace dmcp {
		using namespace std;
		
		class DiscoverMessage : public core::data::SerializableData, public core::base::Visitable {
			public:
				enum Type {
					UNDEFINED = 0,
					DISCOVER = 1,
					RESPONSE = 2,
				};
			public:
				DiscoverMessage();
		
				DiscoverMessage(
				const Type &val0, 
				const coredata::dmcp::ServerInformation &val1, 
				const std::string &val2
				);
		
				virtual ~DiscoverMessage();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				DiscoverMessage(const DiscoverMessage &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				DiscoverMessage& operator=(const DiscoverMessage &obj);
		
			public:
			public:
				/**
				 * @return type.
				 */
				Type getType() const;
				
				/**
				 * This method sets type.
				 *
				 * @param val Value for type.
				 */
				void setType(const Type &val);
			public:
				/**
				 * @return serverInformation.
				 */
				coredata::dmcp::ServerInformation getServerInformation() const;
				
				/**
				 * This method sets serverInformation.
				 *
				 * @param val Value for serverInformation.
				 */
				void setServerInformation(const coredata::dmcp::ServerInformation &val);
			public:
				/**
				 * @return moduleName.
				 */
				std::string getModuleName() const;
				
				/**
				 * This method sets moduleName.
				 *
				 * @param val Value for moduleName.
				 */
				void setModuleName(const std::string &val);
		
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
			private:
				Type m_type;
			private:
				coredata::dmcp::ServerInformation m_serverInformation;
			private:
				std::string m_moduleName;
		
		};
	} // dmcp
} // coredata

#endif /*COREDATA_DMCP_DISCOVERMESSAGE_H*/
