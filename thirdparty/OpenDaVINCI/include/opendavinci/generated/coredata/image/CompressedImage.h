/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_IMAGE_COMPRESSEDIMAGE_H
#define COREDATA_IMAGE_COMPRESSEDIMAGE_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace coredata {
	namespace image {
		using namespace std;
		
		class CompressedImage : public core::data::SerializableData, public core::base::Visitable {
			public:
				CompressedImage();
		
				CompressedImage(
				const std::string &val0, 
				const uint16_t &val1, 
				const uint16_t &val2, 
				const uint8_t &val3, 
				const uint16_t &val4
				);
		
				virtual ~CompressedImage();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				CompressedImage(const CompressedImage &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				CompressedImage& operator=(const CompressedImage &obj);
		
			public:
				/**
				 * @return name.
				 */
				std::string getName() const;
				
				/**
				 * This method sets name.
				 *
				 * @param val Value for name.
				 */
				void setName(const std::string &val);
			public:
				/**
				 * @return width.
				 */
				uint16_t getWidth() const;
				
				/**
				 * This method sets width.
				 *
				 * @param val Value for width.
				 */
				void setWidth(const uint16_t &val);
			public:
				/**
				 * @return height.
				 */
				uint16_t getHeight() const;
				
				/**
				 * This method sets height.
				 *
				 * @param val Value for height.
				 */
				void setHeight(const uint16_t &val);
			public:
				/**
				 * @return bytesPerPixel.
				 */
				uint8_t getBytesPerPixel() const;
				
				/**
				 * This method sets bytesPerPixel.
				 *
				 * @param val Value for bytesPerPixel.
				 */
				void setBytesPerPixel(const uint8_t &val);
			public:
				/**
				 * @return compressedSize.
				 */
				uint16_t getCompressedSize() const;
				
				/**
				 * This method sets compressedSize.
				 *
				 * @param val Value for compressedSize.
				 */
				void setCompressedSize(const uint16_t &val);
		
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
				std::string m_name;
			private:
				uint16_t m_width;
			private:
				uint16_t m_height;
			private:
				uint8_t m_bytesPerPixel;
			private:
				uint16_t m_compressedSize;
		
		};
	} // image
} // coredata

#endif /*COREDATA_IMAGE_COMPRESSEDIMAGE_H*/
