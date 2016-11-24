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

#include "ConfigDeserialization.h"

namespace lmvp {

RoiBuilder::RoiBuilder()
    : topLeft()
    , bottomRight() {}

RoiBuilder::operator RegionOfInterestGeometry() const {
    return RegionOfInterestGeometry(cv::Rect(topLeft, bottomRight));
}

void RoiBuilder::operator>>(std::istream & stream) {
    // FIXME: does not throw when no bottomRight is supplied
    consumeKeyword<Malformed>(stream, "topLeft");
    topLeft = parsePoint<Malformed>(stream);
    consumeKeyword<Malformed>(stream, ", ");

    consumeKeyword<Malformed>(stream, "bottomRight");
    bottomRight = parsePoint<Malformed>(stream);
}

RoiBuilder::Malformed::Malformed()
    : std::runtime_error("Malformed region of interest string. Valid example:"
                         "\"topLeft(108,330), bottomRight(595,460)\"") {}

/**
 * only exists due to a bug in GCCs -Weffc++
 * http://stackoverflow.com/a/14002558
 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=16166#c3
 */
ScanRegionBuilder::ScanRegionBuilder() : bottomLeft(), topLeft(), topRight(), bottomRight() {}

ScanRegionBuilder::operator ScanRegion() const {
    if(!valid) {
        throw Invalid();
    }

    return ScanRegion(bottomLeft, topLeft, topRight, bottomRight);
}

void ScanRegionBuilder::operator>>(std::istream & stream) {
    consumeKeyword<Malformed>(stream, "bottomLeft");
    bottomLeft = parsePoint<Malformed>(stream);
    consumeKeyword<Malformed>(stream, ", ");

    consumeKeyword<Malformed>(stream, "topLeft");
    topLeft = parsePoint<Malformed>(stream);
    consumeKeyword<Malformed>(stream, ", ");

    consumeKeyword<Malformed>(stream, "topRight");
    topRight = parsePoint<Malformed>(stream);
    consumeKeyword<Malformed>(stream, ", ");

    consumeKeyword<Malformed>(stream, "bottomRight");
    bottomRight = parsePoint<Malformed>(stream);

    valid = true;
}

ScanRegionBuilder::Invalid::Invalid()
    : std::runtime_error("Tried to get a ScanRegion instance from the "
                         "ScanRegionBuilder before supplying data.") {}

ScanRegionBuilder::Malformed::Malformed()
    : std::runtime_error("Malformed scan region string. Valid example: \""
                         "bottomLeft(467,456), topLeft(393,325), "
                         "topRight(482,326), bottomRight(608,420)\"") {}

} /* namespace lmvp */
