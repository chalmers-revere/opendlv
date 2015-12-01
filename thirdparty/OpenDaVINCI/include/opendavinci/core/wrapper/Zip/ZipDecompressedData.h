/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2008 - 2015 Christian Berger, Bernhard Rumpe
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

#ifndef OPENDAVINCI_CORE_WRAPPER_ZIP_ZIPDECOMPRESSEDDATA_H_
#define OPENDAVINCI_CORE_WRAPPER_ZIP_ZIPDECOMPRESSEDDATA_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/DecompressedData.h"
#include "core/strings/StringComparator.h"

#include "core/wrapper/CompressionFactoryWorker.h"
#include "core/wrapper/CompressionLibraryProducts.h"

namespace core {
    namespace wrapper {
        namespace Zip {

            using namespace std;

            /**
             * This class implements an abstract object containing
             * the decompressed contents of a compressed archive.
             *
             * @See DecompressedData.
             */
            class ZipDecompressedData : public DecompressedData {
                private:
                    enum {
                        BUFFER_SIZE = 1024
                    };

                private:
                    friend struct CompressionFactoryWorker<CompressionLibraryZIP>;

                    /**
                     * Constructor.
                     *
                     * @param in Stream to be used for reading the contents.
                     */
                    ZipDecompressedData(istream &in);

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    ZipDecompressedData(const ZipDecompressedData &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    ZipDecompressedData& operator=(const ZipDecompressedData &);

                public:
                    virtual ~ZipDecompressedData();

                    virtual vector<string> getListOfEntries();

                    virtual SharedPointer<istream> getInputStreamFor(const string &entry);

                private:
                    map<string, SharedPointer<istream>, core::strings::StringComparator> m_mapOfDecompressedEntries;

                    /**
                     * This method tries to decompress the given archive and
                     * read the complete content into memory.
                     *
                     * @param in Stream to be used for reading the contents.
                     */
                    void decompressData(istream &in);
            };

        }
    }
} // core::wrapper::Zip

#endif /*OPENDAVINCI_CORE_WRAPPER_ZIP_ZIPDECOMPRESSEDDATA_H_*/
