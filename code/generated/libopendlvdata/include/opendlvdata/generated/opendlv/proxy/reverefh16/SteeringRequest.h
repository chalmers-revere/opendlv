/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_REVEREFH16_STEERINGREQUEST_H
#define OPENDLV_PROXY_REVEREFH16_STEERINGREQUEST_H

#include "opendavinci/odcore/opendavinci.h"


#include "opendavinci/odcore/base/Visitable.h"
#include "opendavinci/odcore/data/SerializableData.h"


namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
			using namespace std;
			
			class SteeringRequest : public odcore::data::SerializableData, public odcore::base::Visitable {
				public:
					SteeringRequest();
			
					SteeringRequest(
					const bool &val0, 
					const double &val1, 
					const double &val2
					);
			
					virtual ~SteeringRequest();
			
					/**
					 * Copy constructor.
					 *
					 * @param obj Reference to an object of this class.
					 */
					SteeringRequest(const SteeringRequest &obj);
			
					/**
					 * Assignment operator.
					 *
					 * @param obj Reference to an object of this class.
					 * @return Reference to this instance.
					 */
					SteeringRequest& operator=(const SteeringRequest &obj);
			
				public:
					/**
					 * @return enableRequest.
					 */
					bool getEnableRequest() const;
					
					/**
					 * This method sets enableRequest.
					 *
					 * @param val Value for enableRequest.
					 */
					void setEnableRequest(const bool &val);
				public:
					/**
					 * @return steeringRoadWheelAngle.
					 */
					double getSteeringRoadWheelAngle() const;
					
					/**
					 * This method sets steeringRoadWheelAngle.
					 *
					 * @param val Value for steeringRoadWheelAngle.
					 */
					void setSteeringRoadWheelAngle(const double &val);
				public:
					/**
					 * @return steeringDeltaTorque.
					 */
					double getSteeringDeltaTorque() const;
					
					/**
					 * This method sets steeringDeltaTorque.
					 *
					 * @param val Value for steeringDeltaTorque.
					 */
					void setSteeringDeltaTorque(const double &val);
			
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
					bool m_enableRequest;
				private:
					double m_steeringRoadWheelAngle;
				private:
					double m_steeringDeltaTorque;
			
			};
		} // reverefh16
	} // proxy
} // opendlv

#endif /*OPENDLV_PROXY_REVEREFH16_STEERINGREQUEST_H*/
