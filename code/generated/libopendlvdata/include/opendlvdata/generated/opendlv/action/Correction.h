/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_ACTION_CORRECTION_H
#define OPENDLV_ACTION_CORRECTION_H

#include "opendavinci/odcore/opendavinci.h"


#include "opendavinci/odcore/base/Visitable.h"
#include "opendavinci/odcore/data/SerializableData.h"

#include "opendavinci/odcore/data/TimeStamp.h"

namespace opendlv {
	namespace action {
		using namespace std;
		
		class Correction : public odcore::data::SerializableData, public odcore::base::Visitable {
			public:
				Correction();
		
				Correction(
				const odcore::data::TimeStamp &val0, 
				const std::string &val1, 
				const bool &val2, 
				const float &val3
				);
		
				virtual ~Correction();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				Correction(const Correction &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				Correction& operator=(const Correction &obj);
		
			public:
				/**
				 * @return startTime.
				 */
				odcore::data::TimeStamp getStartTime() const;
				
				/**
				 * This method sets startTime.
				 *
				 * @param val Value for startTime.
				 */
				void setStartTime(const odcore::data::TimeStamp &val);
			public:
				/**
				 * @return type.
				 */
				std::string getType() const;
				
				/**
				 * This method sets type.
				 *
				 * @param val Value for type.
				 */
				void setType(const std::string &val);
			public:
				/**
				 * @return isInhibitory.
				 */
				bool getIsInhibitory() const;
				
				/**
				 * This method sets isInhibitory.
				 *
				 * @param val Value for isInhibitory.
				 */
				void setIsInhibitory(const bool &val);
			public:
				/**
				 * @return amplitude.
				 */
				float getAmplitude() const;
				
				/**
				 * This method sets amplitude.
				 *
				 * @param val Value for amplitude.
				 */
				void setAmplitude(const float &val);
		
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
				odcore::data::TimeStamp m_startTime;
			private:
				std::string m_type;
			private:
				bool m_isInhibitory;
			private:
				float m_amplitude;
		
		};
	} // action
} // opendlv

#endif /*OPENDLV_ACTION_CORRECTION_H*/
