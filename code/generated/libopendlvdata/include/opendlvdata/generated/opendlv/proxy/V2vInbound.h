/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_V2VINBOUND_H
#define OPENDLV_PROXY_V2VINBOUND_H

#include "opendavinci/odcore/opendavinci.h"


#include "opendavinci/odcore/base/Visitable.h"
#include "opendavinci/odcore/data/SerializableData.h"


namespace opendlv {
	namespace proxy {
		using namespace std;
		
		class V2vInbound : public odcore::data::SerializableData, public odcore::base::Visitable {
			public:
				V2vInbound();
		
				V2vInbound(
				const uint32_t &val0, 
				const std::string &val1
				);
		
				virtual ~V2vInbound();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				V2vInbound(const V2vInbound &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				V2vInbound& operator=(const V2vInbound &obj);
		
			public:
				/**
				 * @return size.
				 */
				uint32_t getSize() const;
				
				/**
				 * This method sets size.
				 *
				 * @param val Value for size.
				 */
				void setSize(const uint32_t &val);
			public:
				/**
				 * @return data.
				 */
				std::string getData() const;
				
				/**
				 * This method sets data.
				 *
				 * @param val Value for data.
				 */
				void setData(const std::string &val);
		
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
				uint32_t m_size;
			private:
				std::string m_data;
		
		};
	} // proxy
} // opendlv

#endif /*OPENDLV_PROXY_V2VINBOUND_H*/
