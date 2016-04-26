/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PERCEPTION_LANEPOSITION_H
#define OPENDLV_PERCEPTION_LANEPOSITION_H

#include "opendavinci/odcore/opendavinci.h"


#include "opendavinci/odcore/base/Visitable.h"
#include "opendavinci/odcore/data/SerializableData.h"


namespace opendlv {
	namespace perception {
		using namespace std;
		
		class LanePosition : public odcore::data::SerializableData, public odcore::base::Visitable {
			public:
				LanePosition();
		
				LanePosition(
				const float &val0, 
				const float &val1
				);
		
				virtual ~LanePosition();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				LanePosition(const LanePosition &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				LanePosition& operator=(const LanePosition &obj);
		
			public:
				/**
				 * @return offset.
				 */
				float getOffset() const;
				
				/**
				 * This method sets offset.
				 *
				 * @param val Value for offset.
				 */
				void setOffset(const float &val);
			public:
				/**
				 * @return heading.
				 */
				float getHeading() const;
				
				/**
				 * This method sets heading.
				 *
				 * @param val Value for heading.
				 */
				void setHeading(const float &val);
		
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
		
				/**
				 * This method returns the message id.
				 *
				 * @return Message id.
				 */
				virtual int32_t getID() const;
		
				/**
				 * This method returns the short message name.
				 *
				 * @return Short message name.
				 */
				virtual const string getShortName() const;
		
				/**
				 * This method returns the long message name include package/sub structure.
				 *
				 * @return Long message name.
				 */
				virtual const string getLongName() const;
		
			public:
				virtual void accept(odcore::base::Visitor &v);
		
				virtual ostream& operator<<(ostream &out) const;
				virtual istream& operator>>(istream &in);
		
				virtual const string toString() const;
		
			private:
				float m_offset;
			private:
				float m_heading;
		
		};
	} // perception
} // opendlv

#endif /*OPENDLV_PERCEPTION_LANEPOSITION_H*/
