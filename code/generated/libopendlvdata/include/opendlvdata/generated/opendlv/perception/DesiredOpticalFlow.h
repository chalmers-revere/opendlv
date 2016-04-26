/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PERCEPTION_DESIREDOPTICALFLOW_H
#define OPENDLV_PERCEPTION_DESIREDOPTICALFLOW_H

#include "opendavinci/odcore/opendavinci.h"


#include "opendavinci/odcore/base/Visitable.h"
#include "opendavinci/odcore/data/SerializableData.h"


namespace opendlv {
	namespace perception {
		using namespace std;
		
		class DesiredOpticalFlow : public odcore::data::SerializableData, public odcore::base::Visitable {
			public:
				DesiredOpticalFlow();
		
				DesiredOpticalFlow(
				const float &val0
				);
		
				virtual ~DesiredOpticalFlow();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				DesiredOpticalFlow(const DesiredOpticalFlow &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				DesiredOpticalFlow& operator=(const DesiredOpticalFlow &obj);
		
			public:
				/**
				 * @return flow.
				 */
				float getFlow() const;
				
				/**
				 * This method sets flow.
				 *
				 * @param val Value for flow.
				 */
				void setFlow(const float &val);
		
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
				float m_flow;
		
		};
	} // perception
} // opendlv

#endif /*OPENDLV_PERCEPTION_DESIREDOPTICALFLOW_H*/
