/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_REVEREFH16_STEERING_H
#define OPENDLV_PROXY_REVEREFH16_STEERING_H

#include "opendavinci/odcore/opendavinci.h"


#include "opendavinci/odcore/base/Visitable.h"
#include "opendavinci/odcore/data/SerializableData.h"


namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
			using namespace std;
			
			class Steering : public odcore::data::SerializableData, public odcore::base::Visitable {
				public:
					Steering();
			
					Steering(
					const double &val0, 
					const double &val1
					);
			
					virtual ~Steering();
			
					/**
					 * Copy constructor.
					 *
					 * @param obj Reference to an object of this class.
					 */
					Steering(const Steering &obj);
			
					/**
					 * Assignment operator.
					 *
					 * @param obj Reference to an object of this class.
					 * @return Reference to this instance.
					 */
					Steering& operator=(const Steering &obj);
			
				public:
					/**
					 * @return roadwheelangle.
					 */
					double getRoadwheelangle() const;
					
					/**
					 * This method sets roadwheelangle.
					 *
					 * @param val Value for roadwheelangle.
					 */
					void setRoadwheelangle(const double &val);
				public:
					/**
					 * @return steeringwheelangle.
					 */
					double getSteeringwheelangle() const;
					
					/**
					 * This method sets steeringwheelangle.
					 *
					 * @param val Value for steeringwheelangle.
					 */
					void setSteeringwheelangle(const double &val);
			
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
					double m_roadwheelangle;
				private:
					double m_steeringwheelangle;
			
			};
		} // reverefh16
	} // proxy
} // opendlv

#endif /*OPENDLV_PROXY_REVEREFH16_STEERING_H*/
