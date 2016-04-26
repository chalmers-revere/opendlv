/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_REVEREFH16_AXLES_H
#define OPENDLV_PROXY_REVEREFH16_AXLES_H

#include "opendavinci/odcore/opendavinci.h"


#include "opendavinci/odcore/base/Visitable.h"
#include "opendavinci/odcore/data/SerializableData.h"


namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
			using namespace std;
			
			class Axles : public odcore::data::SerializableData, public odcore::base::Visitable {
				public:
					Axles();
			
					Axles(
					const double &val0, 
					const double &val1, 
					const double &val2
					);
			
					virtual ~Axles();
			
					/**
					 * Copy constructor.
					 *
					 * @param obj Reference to an object of this class.
					 */
					Axles(const Axles &obj);
			
					/**
					 * Assignment operator.
					 *
					 * @param obj Reference to an object of this class.
					 * @return Reference to this instance.
					 */
					Axles& operator=(const Axles &obj);
			
				public:
					/**
					 * @return loadAxle11.
					 */
					double getLoadAxle11() const;
					
					/**
					 * This method sets loadAxle11.
					 *
					 * @param val Value for loadAxle11.
					 */
					void setLoadAxle11(const double &val);
				public:
					/**
					 * @return loadAxle12.
					 */
					double getLoadAxle12() const;
					
					/**
					 * This method sets loadAxle12.
					 *
					 * @param val Value for loadAxle12.
					 */
					void setLoadAxle12(const double &val);
				public:
					/**
					 * @return loadAxle13.
					 */
					double getLoadAxle13() const;
					
					/**
					 * This method sets loadAxle13.
					 *
					 * @param val Value for loadAxle13.
					 */
					void setLoadAxle13(const double &val);
			
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
					double m_loadAxle11;
				private:
					double m_loadAxle12;
				private:
					double m_loadAxle13;
			
			};
		} // reverefh16
	} // proxy
} // opendlv

#endif /*OPENDLV_PROXY_REVEREFH16_AXLES_H*/
