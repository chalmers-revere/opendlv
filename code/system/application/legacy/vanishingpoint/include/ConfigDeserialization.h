/*
 * Copyright (C) 2016 Crispin Kirchner
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * The classes in this file serve for reading the complex configuration objects of the vanishing
 * point algorithm from single entries of the module's keyValueConfiguration, for better
 * readability of the latter.
 */

#ifndef OPENDLV_LEGACY_VANISHINGPOINT_CONFIG_DESERIALIZATION_H_
#define OPENDLV_LEGACY_VANISHINGPOINT_CONFIG_DESERIALIZATION_H_

#include <istream>
#include <stdexcept>

#include <opencv2/core/core.hpp>

#include "RegionOfInterestGeometry.h"
#include "ScanRegion.h"

namespace opendlv {
namespace legacy {
namespace vanishingpoint {

/**
 * Class containing the common parts of the two builders.
 */
template<class T>
class BuilderParser {
public:
    virtual ~BuilderParser() = default;
    virtual operator T() const = 0;
    virtual void operator>>(std::istream &) = 0;

protected:
    template<class E>
    static void consumeKeyword(std::istream & stream, const std::string & keyword) {
        std::string keywordBuffer(keyword);

        stream.read(&keywordBuffer[0], keyword.length());

        if(0 != keywordBuffer.compare(keyword)) {
            throw E();
        }
    }

    template<class E>
    static cv::Point parsePoint(std::istream & stream) {
        cv::Point point;

        consumeKeyword<E>(stream, "(");
        stream >> point.x;
        consumeKeyword<E>(stream, ",");
        stream >> point.y;
        consumeKeyword<E>(stream, ")");

        return point;
    }
};

template<class T>
std::istream & operator>>(std::istream & stream, BuilderParser<T> & builder) {
    builder.operator>>(stream);

    return stream;
}

class RoiBuilder : public BuilderParser<RegionOfInterestGeometry> {
public:
    RoiBuilder();

    operator RegionOfInterestGeometry() const;

    void operator>>(std::istream & stream);

    class Malformed : std::runtime_error {
    public:
        Malformed();
    };

private:
    cv::Point topLeft, bottomRight;
};

class ScanRegionBuilder : public BuilderParser<ScanRegion> {
public:
    ScanRegionBuilder();

    /**
     * Get the result of building after reading in the string representation.
     */
    operator ScanRegion() const;

    void operator>>(std::istream & stream);

    class Invalid : std::runtime_error {
    public:
        Invalid();
    };

    class Malformed : std::runtime_error {
    public:
        Malformed();
    };

private:
    bool valid = false;
    cv::Point bottomLeft, topLeft, topRight, bottomRight;
};

}
}
}

#endif
