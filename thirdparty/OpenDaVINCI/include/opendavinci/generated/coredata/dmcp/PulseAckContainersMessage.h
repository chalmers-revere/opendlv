/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_DMCP_PULSEACKCONTAINERSMESSAGE_H
#define COREDATA_DMCP_PULSEACKCONTAINERSMESSAGE_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include <vector>

#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"

#include "core/data/Container.h"

namespace coredata {
	namespace dmcp {
		using namespace std;
		
		class PulseAckContainersMessage : public core::data::SerializableData, public core::base::Visitable {
			public:
				PulseAckContainersMessage();
		
				PulseAckContainersMessage(
				const vector<core::data::Container> &val0
				);
		
				virtual ~PulseAckContainersMessage();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				PulseAckContainersMessage(const PulseAckContainersMessage &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				PulseAckContainersMessage& operator=(const PulseAckContainersMessage &obj);
		
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
				std::vector<core::data::Container> m_listOfContainers;
		
		};
	} // dmcp
} // coredata

#endif /*COREDATA_DMCP_PULSEACKCONTAINERSMESSAGE_H*/
