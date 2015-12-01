/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_BUFFER_MEMORYSEGMENT_H
#define COREDATA_BUFFER_MEMORYSEGMENT_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"

#include "core/data/Container.h"

namespace coredata {
	namespace buffer {
		using namespace std;
		
		class MemorySegment : public core::data::SerializableData, public core::base::Visitable {
			public:
				MemorySegment();
		
				MemorySegment(
				const core::data::Container &val0, 
				const uint32_t &val1, 
				const uint32_t &val2, 
				const uint16_t &val3
				);
		
				virtual ~MemorySegment();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				MemorySegment(const MemorySegment &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				MemorySegment& operator=(const MemorySegment &obj);
		
			public:
				/**
				 * @return header.
				 */
				core::data::Container getHeader() const;
				
				/**
				 * This method sets header.
				 *
				 * @param val Value for header.
				 */
				void setHeader(const core::data::Container &val);
			public:
				/**
				 * @return size.
				 */
				uint32_t getSize() const;
				
				/**
				 * This method sets size.
				 *
				 * @param val Value for size.
				 */
				void setSize(const uint32_t &val);
			public:
				/**
				 * @return consumedSize.
				 */
				uint32_t getConsumedSize() const;
				
				/**
				 * This method sets consumedSize.
				 *
				 * @param val Value for consumedSize.
				 */
				void setConsumedSize(const uint32_t &val);
			public:
				/**
				 * @return identifier.
				 */
				uint16_t getIdentifier() const;
				
				/**
				 * This method sets identifier.
				 *
				 * @param val Value for identifier.
				 */
				void setIdentifier(const uint16_t &val);
		
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
				core::data::Container m_header;
			private:
				uint32_t m_size;
			private:
				uint32_t m_consumedSize;
			private:
				uint16_t m_identifier;
		
		};
	} // buffer
} // coredata

#endif /*COREDATA_BUFFER_MEMORYSEGMENT_H*/
