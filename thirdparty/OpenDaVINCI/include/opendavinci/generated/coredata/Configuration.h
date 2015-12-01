/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_CONFIGURATION_H
#define COREDATA_CONFIGURATION_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"

#include "core/base/KeyValueConfiguration.h"

namespace coredata {
	using namespace std;
	
	class Configuration : public core::data::SerializableData, public core::base::Visitable {
		public:
			Configuration();
	
			Configuration(
			const core::base::KeyValueConfiguration &val0
			);
	
			virtual ~Configuration();
	
			/**
			 * Copy constructor.
			 *
			 * @param obj Reference to an object of this class.
			 */
			Configuration(const Configuration &obj);
	
			/**
			 * Assignment operator.
			 *
			 * @param obj Reference to an object of this class.
			 * @return Reference to this instance.
			 */
			Configuration& operator=(const Configuration &obj);
	
		public:
			/**
			 * @return keyValueConfiguration.
			 */
			core::base::KeyValueConfiguration getKeyValueConfiguration() const;
			
			/**
			 * This method sets keyValueConfiguration.
			 *
			 * @param val Value for keyValueConfiguration.
			 */
			void setKeyValueConfiguration(const core::base::KeyValueConfiguration &val);
	
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
			core::base::KeyValueConfiguration m_keyValueConfiguration;
	
	};
} // coredata

#endif /*COREDATA_CONFIGURATION_H*/
