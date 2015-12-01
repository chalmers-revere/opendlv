/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_DMCP_MODULEEXITCODEMESSAGE_H
#define COREDATA_DMCP_MODULEEXITCODEMESSAGE_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace coredata {
	namespace dmcp {
		using namespace std;
		
		class ModuleExitCodeMessage : public core::data::SerializableData, public core::base::Visitable {
			public:
				enum ModuleExitCode {
					NO_SUPERCOMPONENT = 4,
					OKAY = 0,
					UNDEFINED_EXITCODE = 5,
					CONNECTION_LOST = 3,
					SERIOUS_ERROR = 2,
					EXCEPTION_CAUGHT = 1,
				};
			public:
				ModuleExitCodeMessage();
		
				ModuleExitCodeMessage(
				const ModuleExitCode &val0
				);
		
				virtual ~ModuleExitCodeMessage();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				ModuleExitCodeMessage(const ModuleExitCodeMessage &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				ModuleExitCodeMessage& operator=(const ModuleExitCodeMessage &obj);
		
			public:
			public:
				/**
				 * @return moduleExitCode.
				 */
				ModuleExitCode getModuleExitCode() const;
				
				/**
				 * This method sets moduleExitCode.
				 *
				 * @param val Value for moduleExitCode.
				 */
				void setModuleExitCode(const ModuleExitCode &val);
		
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
				ModuleExitCode m_moduleExitCode;
		
		};
	} // dmcp
} // coredata

#endif /*COREDATA_DMCP_MODULEEXITCODEMESSAGE_H*/
