/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_REVEREFH16_WHEELS_H
#define OPENDLV_PROXY_REVEREFH16_WHEELS_H

#include "opendavinci/odcore/opendavinci.h"


#include "opendavinci/odcore/base/Visitable.h"
#include "opendavinci/odcore/data/SerializableData.h"


namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
			using namespace std;
			
			class Wheels : public odcore::data::SerializableData, public odcore::base::Visitable {
				public:
					Wheels();
			
					Wheels(
					const double &val0, 
					const double &val1, 
					const double &val2, 
					const double &val3, 
					const double &val4, 
					const double &val5
					);
			
					virtual ~Wheels();
			
					/**
					 * Copy constructor.
					 *
					 * @param obj Reference to an object of this class.
					 */
					Wheels(const Wheels &obj);
			
					/**
					 * Assignment operator.
					 *
					 * @param obj Reference to an object of this class.
					 * @return Reference to this instance.
					 */
					Wheels& operator=(const Wheels &obj);
			
				public:
					/**
					 * @return speedWheel111.
					 */
					double getSpeedWheel111() const;
					
					/**
					 * This method sets speedWheel111.
					 *
					 * @param val Value for speedWheel111.
					 */
					void setSpeedWheel111(const double &val);
				public:
					/**
					 * @return speedWheel112.
					 */
					double getSpeedWheel112() const;
					
					/**
					 * This method sets speedWheel112.
					 *
					 * @param val Value for speedWheel112.
					 */
					void setSpeedWheel112(const double &val);
				public:
					/**
					 * @return speedWheel121.
					 */
					double getSpeedWheel121() const;
					
					/**
					 * This method sets speedWheel121.
					 *
					 * @param val Value for speedWheel121.
					 */
					void setSpeedWheel121(const double &val);
				public:
					/**
					 * @return speedWheel122.
					 */
					double getSpeedWheel122() const;
					
					/**
					 * This method sets speedWheel122.
					 *
					 * @param val Value for speedWheel122.
					 */
					void setSpeedWheel122(const double &val);
				public:
					/**
					 * @return speedWheel131.
					 */
					double getSpeedWheel131() const;
					
					/**
					 * This method sets speedWheel131.
					 *
					 * @param val Value for speedWheel131.
					 */
					void setSpeedWheel131(const double &val);
				public:
					/**
					 * @return speedWheel132.
					 */
					double getSpeedWheel132() const;
					
					/**
					 * This method sets speedWheel132.
					 *
					 * @param val Value for speedWheel132.
					 */
					void setSpeedWheel132(const double &val);
			
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
					double m_speedWheel111;
				private:
					double m_speedWheel112;
				private:
					double m_speedWheel121;
				private:
					double m_speedWheel122;
				private:
					double m_speedWheel131;
				private:
					double m_speedWheel132;
			
			};
		} // reverefh16
	} // proxy
} // opendlv

#endif /*OPENDLV_PROXY_REVEREFH16_WHEELS_H*/
