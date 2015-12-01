/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_IMAGE_SHAREDIMAGE_H
#define COREDATA_IMAGE_SHAREDIMAGE_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


#include "GeneratedHeaders_CoreData.h"
namespace coredata {
	namespace image {
		using namespace std;
		
		class SharedImage : public coredata::SharedData {
			public:
				SharedImage();
		
				SharedImage(
				const uint32_t &val0, 
				const uint32_t &val1, 
				const uint32_t &val2
				);
		
				virtual ~SharedImage();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				SharedImage(const SharedImage &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				SharedImage& operator=(const SharedImage &obj);
		
			public:
				/**
				 * @return width.
				 */
				uint32_t getWidth() const;
				
				/**
				 * This method sets width.
				 *
				 * @param val Value for width.
				 */
				void setWidth(const uint32_t &val);
			public:
				/**
				 * @return height.
				 */
				uint32_t getHeight() const;
				
				/**
				 * This method sets height.
				 *
				 * @param val Value for height.
				 */
				void setHeight(const uint32_t &val);
			public:
				/**
				 * @return bytesPerPixel.
				 */
				uint32_t getBytesPerPixel() const;
				
				/**
				 * This method sets bytesPerPixel.
				 *
				 * @param val Value for bytesPerPixel.
				 */
				void setBytesPerPixel(const uint32_t &val);
		
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
				uint32_t m_width;
			private:
				uint32_t m_height;
			private:
				uint32_t m_bytesPerPixel;
		
		};
	} // image
} // coredata

#endif /*COREDATA_IMAGE_SHAREDIMAGE_H*/
