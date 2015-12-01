/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_DMCP_MODULESTATISTIC_H
#define COREDATA_DMCP_MODULESTATISTIC_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"

#include "generated/coredata/dmcp/ModuleDescriptor.h"
#include "generated/coredata/dmcp/RuntimeStatistic.h"

namespace coredata {
	namespace dmcp {
		using namespace std;
		
		class ModuleStatistic : public core::data::SerializableData, public core::base::Visitable {
			public:
				ModuleStatistic();
		
				ModuleStatistic(
				const coredata::dmcp::ModuleDescriptor &val0, 
				const coredata::dmcp::RuntimeStatistic &val1
				);
		
				virtual ~ModuleStatistic();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				ModuleStatistic(const ModuleStatistic &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				ModuleStatistic& operator=(const ModuleStatistic &obj);
		
			public:
				/**
				 * @return module.
				 */
				coredata::dmcp::ModuleDescriptor getModule() const;
				
				/**
				 * This method sets module.
				 *
				 * @param val Value for module.
				 */
				void setModule(const coredata::dmcp::ModuleDescriptor &val);
			public:
				/**
				 * @return runtimeStatistic.
				 */
				coredata::dmcp::RuntimeStatistic getRuntimeStatistic() const;
				
				/**
				 * This method sets runtimeStatistic.
				 *
				 * @param val Value for runtimeStatistic.
				 */
				void setRuntimeStatistic(const coredata::dmcp::RuntimeStatistic &val);
		
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
				coredata::dmcp::ModuleDescriptor m_module;
			private:
				coredata::dmcp::RuntimeStatistic m_runtimeStatistic;
		
		};
	} // dmcp
} // coredata

#endif /*COREDATA_DMCP_MODULESTATISTIC_H*/
