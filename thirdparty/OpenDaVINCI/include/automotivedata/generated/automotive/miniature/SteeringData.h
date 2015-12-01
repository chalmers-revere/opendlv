/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef AUTOMOTIVE_MINIATURE_STEERINGDATA_H
#define AUTOMOTIVE_MINIATURE_STEERINGDATA_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace automotive {
	namespace miniature {
		using namespace std;
		
		class SteeringData : public core::data::SerializableData, public core::base::Visitable {
			public:
				SteeringData();
		
				SteeringData(
				const double &val0
				);
		
				virtual ~SteeringData();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				SteeringData(const SteeringData &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				SteeringData& operator=(const SteeringData &obj);
		
			public:
				/**
				 * @return exampleData.
				 */
				double getExampleData() const;
				
				/**
				 * This method sets exampleData.
				 *
				 * @param val Value for exampleData.
				 */
				void setExampleData(const double &val);
		
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
				double m_exampleData;
		
		};
	} // miniature
} // automotive

#endif /*AUTOMOTIVE_MINIATURE_STEERINGDATA_H*/
