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

namespace opendlv {
namespace legacy {
namespace vanishingpoint {

template<class T>
template<class E>
void BuilderParser<T>::consumeKeyword(std::istream & stream, const std::string & keyword) {
    std::string keywordBuffer(keyword);

    stream.read(&keywordBuffer[0], keyword.length());

    if(0 != keywordBuffer.compare(keyword)) {
        throw E();
    }
}

template<class T>
template<class E>
cv::Point BuilderParser<T>::parsePoint(std::istream & stream) {
    cv::Point point;

    consumeKeyword<E>(stream, "(");
    stream >> point.x;
    consumeKeyword<E>(stream, ",");
    stream >> point.y;
    consumeKeyword<E>(stream, ")");

    return point;
}

template<class T>
std::istream & operator>>(std::istream & stream, BuilderParser<T> & builder) {
    builder.operator>>(stream);

    return stream;
}

}
}
}
