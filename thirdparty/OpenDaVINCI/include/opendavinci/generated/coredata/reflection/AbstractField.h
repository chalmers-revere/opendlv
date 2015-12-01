/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_REFLECTION_ABSTRACTFIELD_H
#define COREDATA_REFLECTION_ABSTRACTFIELD_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace coredata {
	namespace reflection {
		using namespace std;
		
		class AbstractField : public core::data::SerializableData, public core::base::Visitable {
			public:
				enum FIELDDATATYPE {
					BOOL_T = 0,
					CHAR_T = 11,
					STRING_T = 51,
					UINT8_T = 2,
					DOUBLE_T = 14,
					NON_PRIMITIVE_END = 54,
					NON_PRIMITIVE_START = 50,
					FLOAT_T = 13,
					UINT16_T = 4,
					UINT64_T = 8,
					SERIALIZABLE_T = 53,
					UINT32_T = 6,
					INTEGERS_START = 1,
					INT32_T = 7,
					INT64_T = 9,
					UCHAR_T = 12,
					DATA_T = 52,
					INT8_T = 3,
					INT16_T = 5,
					INTEGERS_END = 10,
				};
			public:
				AbstractField();
		
				AbstractField(
				const uint32_t &val0, 
				const uint8_t &val1, 
				const std::string &val2, 
				const std::string &val3, 
				const FIELDDATATYPE &val4, 
				const int8_t &val5
				);
		
				virtual ~AbstractField();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				AbstractField(const AbstractField &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				AbstractField& operator=(const AbstractField &obj);
		
			public:
			public:
				/**
				 * @return longIdentifier.
				 */
				uint32_t getLongIdentifier() const;
				
				/**
				 * This method sets longIdentifier.
				 *
				 * @param val Value for longIdentifier.
				 */
				void setLongIdentifier(const uint32_t &val);
			public:
				/**
				 * @return shortIdentifier.
				 */
				uint8_t getShortIdentifier() const;
				
				/**
				 * This method sets shortIdentifier.
				 *
				 * @param val Value for shortIdentifier.
				 */
				void setShortIdentifier(const uint8_t &val);
			public:
				/**
				 * @return longName.
				 */
				std::string getLongName() const;
				
				/**
				 * This method sets longName.
				 *
				 * @param val Value for longName.
				 */
				void setLongName(const std::string &val);
			public:
				/**
				 * @return shortName.
				 */
				std::string getShortName() const;
				
				/**
				 * This method sets shortName.
				 *
				 * @param val Value for shortName.
				 */
				void setShortName(const std::string &val);
			public:
				/**
				 * @return fieldDataType.
				 */
				FIELDDATATYPE getFieldDataType() const;
				
				/**
				 * This method sets fieldDataType.
				 *
				 * @param val Value for fieldDataType.
				 */
				void setFieldDataType(const FIELDDATATYPE &val);
			public:
				/**
				 * @return size.
				 */
				int8_t getSize() const;
				
				/**
				 * This method sets size.
				 *
				 * @param val Value for size.
				 */
				void setSize(const int8_t &val);
		
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
		
			public:
				virtual void accept(core::base::Visitor &v);
		
				virtual ostream& operator<<(ostream &out) const;
				virtual istream& operator>>(istream &in);
		
				virtual const string toString() const;
		
			private:
			private:
				uint32_t m_longIdentifier;
			private:
				uint8_t m_shortIdentifier;
			private:
				std::string m_longName;
			private:
				std::string m_shortName;
			private:
				FIELDDATATYPE m_fieldDataType;
			private:
				int8_t m_size;
		
		};
	} // reflection
} // coredata

#endif /*COREDATA_REFLECTION_ABSTRACTFIELD_H*/
