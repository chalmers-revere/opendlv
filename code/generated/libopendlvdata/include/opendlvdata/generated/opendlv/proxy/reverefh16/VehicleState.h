/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_REVEREFH16_VEHICLESTATE_H
#define OPENDLV_PROXY_REVEREFH16_VEHICLESTATE_H

#include "opendavinci/odcore/opendavinci.h"


#include "opendavinci/odcore/base/Visitable.h"
#include "opendavinci/odcore/data/SerializableData.h"


namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
			using namespace std;
			
			class VehicleState : public odcore::data::SerializableData, public odcore::base::Visitable {
				public:
					VehicleState();
			
					VehicleState(
					const double &val0, 
					const double &val1, 
					const double &val2
					);
			
					virtual ~VehicleState();
			
					/**
					 * Copy constructor.
					 *
					 * @param obj Reference to an object of this class.
					 */
					VehicleState(const VehicleState &obj);
			
					/**
					 * Assignment operator.
					 *
					 * @param obj Reference to an object of this class.
					 * @return Reference to this instance.
					 */
					VehicleState& operator=(const VehicleState &obj);
			
				public:
					/**
					 * @return accelerationX.
					 */
					double getAccelerationX() const;
					
					/**
					 * This method sets accelerationX.
					 *
					 * @param val Value for accelerationX.
					 */
					void setAccelerationX(const double &val);
				public:
					/**
					 * @return accelerationY.
					 */
					double getAccelerationY() const;
					
					/**
					 * This method sets accelerationY.
					 *
					 * @param val Value for accelerationY.
					 */
					void setAccelerationY(const double &val);
				public:
					/**
					 * @return yawRate.
					 */
					double getYawRate() const;
					
					/**
					 * This method sets yawRate.
					 *
					 * @param val Value for yawRate.
					 */
					void setYawRate(const double &val);
			
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
					double m_accelerationX;
				private:
					double m_accelerationY;
				private:
					double m_yawRate;
			
			};
		} // reverefh16
	} // proxy
} // opendlv

#endif /*OPENDLV_PROXY_REVEREFH16_VEHICLESTATE_H*/
