/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_REVEREFH16_PROPULSION_H
#define OPENDLV_PROXY_REVEREFH16_PROPULSION_H

#include "opendavinci/odcore/opendavinci.h"


#include "opendavinci/odcore/base/Visitable.h"
#include "opendavinci/odcore/data/SerializableData.h"


namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
			using namespace std;
			
			class Propulsion : public odcore::data::SerializableData, public odcore::base::Visitable {
				public:
					Propulsion();
			
					Propulsion(
					const double &val0
					);
			
					virtual ~Propulsion();
			
					/**
					 * Copy constructor.
					 *
					 * @param obj Reference to an object of this class.
					 */
					Propulsion(const Propulsion &obj);
			
					/**
					 * Assignment operator.
					 *
					 * @param obj Reference to an object of this class.
					 * @return Reference to this instance.
					 */
					Propulsion& operator=(const Propulsion &obj);
			
				public:
					/**
					 * @return propulsionShaftVehicleSpeed.
					 */
					double getPropulsionShaftVehicleSpeed() const;
					
					/**
					 * This method sets propulsionShaftVehicleSpeed.
					 *
					 * @param val Value for propulsionShaftVehicleSpeed.
					 */
					void setPropulsionShaftVehicleSpeed(const double &val);
			
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
					double m_propulsionShaftVehicleSpeed;
			
			};
		} // reverefh16
	} // proxy
} // opendlv

#endif /*OPENDLV_PROXY_REVEREFH16_PROPULSION_H*/
