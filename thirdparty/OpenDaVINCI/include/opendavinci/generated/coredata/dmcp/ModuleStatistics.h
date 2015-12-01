/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_DMCP_MODULESTATISTICS_H
#define COREDATA_DMCP_MODULESTATISTICS_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include <map>

#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"

#include "generated/coredata/dmcp/ModuleStatistic.h"

namespace coredata {
	namespace dmcp {
		using namespace std;
		
		class ModuleStatistics : public core::data::SerializableData, public core::base::Visitable {
			public:
				ModuleStatistics();
		
				ModuleStatistics(
				const map<std::string, coredata::dmcp::ModuleStatistic> &val0
				);
		
				virtual ~ModuleStatistics();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				ModuleStatistics(const ModuleStatistics &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				ModuleStatistics& operator=(const ModuleStatistics &obj);
		
			public:
				/**
				 * @return moduleStatistics.
				 */
				std::map<std::string, coredata::dmcp::ModuleStatistic> getMapOfModuleStatistics() const;
			
				/**
				 * This method sets moduleStatistics.
				 *
				 * @param val Value for moduleStatistics.
				 */
				void setMapOfModuleStatistics(const std::map<std::string, coredata::dmcp::ModuleStatistic> &val);
			
				/**
				 * This method clears the map of moduleStatistics.
				 */
				void clear_MapOfModuleStatistics();
			
				/**
				 * @return the size of the map of moduleStatistics.
				 */
				uint32_t getSize_MapOfModuleStatistics() const;
			
				/**
				 * @return true if the map of moduleStatistics is empty.
				 */
				bool isEmpty_MapOfModuleStatistics() const;
			
				/**
				 * This method adds an element to the map of moduleStatistics.
				 *
				 * @param key Key of the key/value pair to be added to the map of moduleStatistics.
				 * @param val Value of the key/value pair to be added to the map of moduleStatistics.
				 */
				void putTo_MapOfModuleStatistics(const std::string &key, const coredata::dmcp::ModuleStatistic &val);
			
				/**
				 * @return true if the map of moduleStatistics is contains the given key.
				 */
				bool containsKey_MapOfModuleStatistics(const std::string &key) const;
			
				/**
				 * This method returns the value to the given key. Make sure to check if the given key exists otherwise a new key/value pair will be added (cf. C++ standard).
				 *
				 * @param key Key for which the value should be returned from the map of moduleStatistics.
				 * @return Value to given key from the map of moduleStatistics is contains the given key.
				 */
				coredata::dmcp::ModuleStatistic getValueForKey_MapOfModuleStatistics(const std::string &key);
			
				/**
				 * @return Pair of iterators for the begin and end of the map of moduleStatistics.
				 */
				std::pair<std::map<std::string, coredata::dmcp::ModuleStatistic>::iterator, std::map<std::string, coredata::dmcp::ModuleStatistic>::iterator> iteratorPair_MapOfModuleStatistics();
		
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
				std::map<std::string, coredata::dmcp::ModuleStatistic> m_mapOfModuleStatistics;
		
		};
	} // dmcp
} // coredata

#endif /*COREDATA_DMCP_MODULESTATISTICS_H*/
