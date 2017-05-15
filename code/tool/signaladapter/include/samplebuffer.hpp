/**
 * Copyright (C) 2017 Chalmers Revere
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#ifndef TOOL_SIGNALADAPTER_SAMPLEBUFFER_HPP
#define TOOL_SIGNALADAPTER_SAMPLEBUFFER_HPP

#include <memory>
#include <string>
#include <vector>

namespace opendlv {
namespace tool {

class SampleBuffer {
  public:
    class Iterator {
      public:
        explicit Iterator(SampleBuffer const *);
        Iterator(Iterator const &) = delete;
        Iterator &operator=(Iterator const &) = delete;
        virtual ~Iterator();
        bool ReadBoolean();
        uint8_t ReadByte();
        std::shared_ptr<std::vector<uint8_t>> ReadBytes();
        float ReadFloat32();
        double ReadFloat64();
        int8_t ReadInteger8();
        int16_t ReadInteger16();
        int32_t ReadInteger32();
        int64_t ReadInteger64();
        std::string ReadString();
        void Reset();

      private:
        void CheckOverflow(uint32_t);
        bool HasBytesLeft(uint32_t);
   
        SampleBuffer const *m_outer_buffer;
        uint32_t m_read_pos;
    };



    SampleBuffer();
    explicit SampleBuffer(std::vector<uint8_t> const &);
    SampleBuffer(SampleBuffer const &) = delete;
    SampleBuffer &operator=(SampleBuffer const &) = delete;
    virtual ~SampleBuffer();
    void AppendBoolean(bool);
    void AppendByte(uint8_t);
    void AppendBytes(std::vector<uint8_t> const &);
    void AppendBytesRaw(std::vector<uint8_t> const &);
    void AppendFloat32(float);
    void AppendFloat64(double);
    void AppendInteger8(int8_t);
    void AppendInteger16(int16_t);
    void AppendInteger32(int32_t);
    void AppendInteger64(int64_t);
    void AppendString(std::string const &);
    void AppendStringRaw(std::string const &);
    std::vector<uint8_t> const &GetData() const;
    std::string const GetDataString() const;
    std::shared_ptr<SampleBuffer::Iterator> GetIterator() const;
    uint32_t GetSize() const;

  private:
    void Append(std::vector<uint8_t> const &);

    std::vector<uint8_t> m_bytes;
};

}
}

#endif
