/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_DMCP_MODULESTATEMESSAGE_H
#define COREDATA_DMCP_MODULESTATEMESSAGE_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace coredata {
	namespace dmcp {
		using namespace std;
		
		class ModuleStateMessage : public core::data::SerializableData, public core::base::Visitable {
			public:
				enum ModuleState {
					NOT_RUNNING = 0,
					RUNNING = 1,
					UNDEFINED_STATE = 2,
				};
			public:
				ModuleStateMessage();
		
				ModuleStateMessage(
				const ModuleState &val0
				);
		
				virtual ~ModuleStateMessage();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				ModuleStateMessage(const ModuleStateMessage &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				ModuleStateMessage& operator=(const ModuleStateMessage &obj);
		
			public:
			public:
				/**
				 * @return moduleState.
				 */
				ModuleState getModuleState() const;
				
				/**
				 * This method sets moduleState.
				 *
				 * @param val Value for moduleState.
				 */
				void setModuleState(const ModuleState &val);
		
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
				ModuleState m_moduleState;
		
		};
	} // dmcp
} // coredata

#endif /*COREDATA_DMCP_MODULESTATEMESSAGE_H*/
