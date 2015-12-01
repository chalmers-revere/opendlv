/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_DMCP_MODULEDESCRIPTOR_H
#define COREDATA_DMCP_MODULEDESCRIPTOR_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace coredata {
	namespace dmcp {
		using namespace std;
		
		class ModuleDescriptor : public core::data::SerializableData, public core::base::Visitable {
			public:
				ModuleDescriptor();
		
				ModuleDescriptor(
				const std::string &val0, 
				const std::string &val1, 
				const std::string &val2, 
				const float &val3
				);
		
				virtual ~ModuleDescriptor();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				ModuleDescriptor(const ModuleDescriptor &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				ModuleDescriptor& operator=(const ModuleDescriptor &obj);
		
			public:
				/**
				 * @return name.
				 */
				std::string getName() const;
				
				/**
				 * This method sets name.
				 *
				 * @param val Value for name.
				 */
				void setName(const std::string &val);
			public:
				/**
				 * @return identifier.
				 */
				std::string getIdentifier() const;
				
				/**
				 * This method sets identifier.
				 *
				 * @param val Value for identifier.
				 */
				void setIdentifier(const std::string &val);
			public:
				/**
				 * @return version.
				 */
				std::string getVersion() const;
				
				/**
				 * This method sets version.
				 *
				 * @param val Value for version.
				 */
				void setVersion(const std::string &val);
			public:
				/**
				 * @return frequency.
				 */
				float getFrequency() const;
				
				/**
				 * This method sets frequency.
				 *
				 * @param val Value for frequency.
				 */
				void setFrequency(const float &val);
		
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
				std::string m_name;
			private:
				std::string m_identifier;
			private:
				std::string m_version;
			private:
				float m_frequency;
		
		};
	} // dmcp
} // coredata

#endif /*COREDATA_DMCP_MODULEDESCRIPTOR_H*/
