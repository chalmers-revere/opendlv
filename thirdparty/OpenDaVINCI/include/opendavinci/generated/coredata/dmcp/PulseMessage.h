/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_DMCP_PULSEMESSAGE_H
#define COREDATA_DMCP_PULSEMESSAGE_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include <vector>

#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"

#include "core/data/TimeStamp.h"
#include "core/data/Container.h"

namespace coredata {
	namespace dmcp {
		using namespace std;
		
		class PulseMessage : public core::data::SerializableData, public core::base::Visitable {
			public:
				PulseMessage();
		
				PulseMessage(
				const core::data::TimeStamp &val0, 
				const uint32_t &val1, 
				const uint32_t &val2, 
				const vector<core::data::Container> &val3
				);
		
				virtual ~PulseMessage();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				PulseMessage(const PulseMessage &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				PulseMessage& operator=(const PulseMessage &obj);
		
			public:
				/**
				 * @return realTimeFromSupercomponent.
				 */
				core::data::TimeStamp getRealTimeFromSupercomponent() const;
				
				/**
				 * This method sets realTimeFromSupercomponent.
				 *
				 * @param val Value for realTimeFromSupercomponent.
				 */
				void setRealTimeFromSupercomponent(const core::data::TimeStamp &val);
			public:
				/**
				 * @return nominalTimeSlice.
				 */
				uint32_t getNominalTimeSlice() const;
				
				/**
				 * This method sets nominalTimeSlice.
				 *
				 * @param val Value for nominalTimeSlice.
				 */
				void setNominalTimeSlice(const uint32_t &val);
			public:
				/**
				 * @return cumulatedTimeSlice.
				 */
				uint32_t getCumulatedTimeSlice() const;
				
				/**
				 * This method sets cumulatedTimeSlice.
				 *
				 * @param val Value for cumulatedTimeSlice.
				 */
				void setCumulatedTimeSlice(const uint32_t &val);
			public:
				/**
				 * @return containers.
				 */
				std::vector<core::data::Container> getListOfContainers() const;
			
				/**
				 * This method sets containers.
				 *
				 * @param val Value for containers.
				 */
				void setListOfContainers(const std::vector<core::data::Container> &val);
			
				/**
				 * This method clears the list of containers.
				 */
				void clear_ListOfContainers();
			
				/**
				 * @return the size of the list of containers.
				 */
				uint32_t getSize_ListOfContainers() const;
			
				/**
				 * @return true if the list of containers is empty.
				 */
				bool isEmpty_ListOfContainers() const;
			
				/**
				 * This method adds an element to the end of the list of containers.
				 *
				 * @param val Value to be added to the end of the list of containers.
				 */
				void addTo_ListOfContainers(const core::data::Container &val);
			
				/**
				 * This method adds an element to the beginning of the list of containers.
				 *
				 * @param val Value to be inserted to the beginning of the list of containers.
				 */
				void insertTo_ListOfContainers(const core::data::Container &val);
			
				/**
				 * @return true if the list of containers contains the element val.
				 *              If the list has a complex data type, the entries are compared using their .toString() method.
				 */
				bool contains_ListOfContainers(const core::data::Container &val) const;
			
				/**
				 * @return Pair of iterators for the begin and end of the list of containers.
				 */
				std::pair<std::vector<core::data::Container>::iterator, std::vector<core::data::Container>::iterator> iteratorPair_ListOfContainers();
		
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
				core::data::TimeStamp m_realTimeFromSupercomponent;
			private:
				uint32_t m_nominalTimeSlice;
			private:
				uint32_t m_cumulatedTimeSlice;
			private:
				std::vector<core::data::Container> m_listOfContainers;
		
		};
	} // dmcp
} // coredata

#endif /*COREDATA_DMCP_PULSEMESSAGE_H*/
