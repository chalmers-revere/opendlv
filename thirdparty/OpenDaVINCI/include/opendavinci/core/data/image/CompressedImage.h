/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2015 Christian Berger
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef OPENDAVINCI_CORE_DATA_IMAGE_COMPRESSEDIMAGE_H_
#define OPENDAVINCI_CORE_DATA_IMAGE_COMPRESSEDIMAGE_H_

#include <string>

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"

#include "core/data/SerializableData.h"

namespace core {
    namespace data {
        namespace image {

            using namespace std;

            /**
             * This class provides information about a compressed image using
             * JPEG encoding based on IJG.
             */
            class OPENDAVINCI_API CompressedImage : public core::data::SerializableData {
                public:
                    CompressedImage();

                    /**
                     * Constructor that allocates the memory to store image data. At disposal,
                     * this memory is released again.
                     *
                     * @param name
                     * @param width
                     * @param height
                     * @param bpp Bytes per pixel
                     * @param compressedSize bytes of the compressed image
                     */
                    CompressedImage(const string &name, const uint32_t &width, const uint32_t &height, const uint32_t &bpp, const uint32_t &compressedSize);

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

                    /**
                     * This method returns the image's name.
                     *
                     * @return Image's name.
                     */
                    const string getName() const;

                    /**
                     * This method sets the image's name.
                     *
                     * @param name Image's name.
                     */
                    void setName(const string &name);

                    /**
                     * This method returns the image's width.
                     *
                     * @return Image's width.
                     */
                    uint32_t getWidth() const;

                    /**
                     * This method sets the image's width.
                     *
                     * @param width Image's width.
                     */
                    void setWidth(const uint32_t &width);

                    /**
                     * This method returns the image's height.
                     *
                     * @return Image's height.
                     */
                    uint32_t getHeight() const;

                    /**
                     * This method sets the image's height.
                     *
                     * @param height Image's height.
                     */
                    void setHeight(const uint32_t &height);

                    /**
                     * This method returns the image's bytes per pixel.
                     *
                     * @return Image's bytes per pixel.
                     */
                    uint32_t getBytesPerPixel() const;

                    /**
                     * This method sets the image's bytes per pixel.
                     *
                     * @param bytesPerPixel Image's bytes per pixel.
                     */
                    void setBytesPerPixel(const uint32_t &bytesPerPixel);

                    /**
                     * This method returns the image's size in bytes.
                     *
                     * @return Image's uncompressed size in bytes.
                     */
                    uint32_t getSize() const;

                    /**
                     * This method returns the image's compressed size in bytes.
                     *
                     * @return Image's compressed size in bytes.
                     */
                    uint32_t getCompressedSize() const;

                    /**
                     * This method returns the pointer to the image's raw data.
                     *
                     * @return Image's bytes per pixel.
                     */
                    uint8_t* getRawData() const;


                    virtual ostream& operator<<(ostream &out) const;
                    virtual istream& operator>>(istream &in);

                    virtual const string toString() const;

                private:
                    string m_name;
                    uint32_t m_width;
                    uint32_t m_height;
                    uint32_t m_bytesPerPixel;
                    uint32_t m_compressedSize;
                    uint8_t *m_imageData;
            };
        }
    }
} // core::data::image

#endif /*OPENDAVINCI_CORE_DATA_IMAGE_COMPRESSEDIMAGE_H_*/
