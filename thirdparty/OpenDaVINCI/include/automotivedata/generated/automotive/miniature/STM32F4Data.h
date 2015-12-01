/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef AUTOMOTIVE_MINIATURE_STM32F4DATA_H
#define AUTOMOTIVE_MINIATURE_STM32F4DATA_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace automotive {
	namespace miniature {
		using namespace std;
		
		class STM32F4Data : public core::data::SerializableData, public core::base::Visitable {
			public:
				STM32F4Data();
		
				STM32F4Data(
				const std::string &val0
				);
		
				virtual ~STM32F4Data();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				STM32F4Data(const STM32F4Data &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				STM32F4Data& operator=(const STM32F4Data &obj);
		
			public:
				/**
				 * @return rawData.
				 */
				std::string getRawData() const;
				
				/**
				 * This method sets rawData.
				 *
				 * @param val Value for rawData.
				 */
				void setRawData(const std::string &val);
		
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
				std::string m_rawData;
		
		};
	} // miniature
} // automotive

#endif /*AUTOMOTIVE_MINIATURE_STM32F4DATA_H*/
