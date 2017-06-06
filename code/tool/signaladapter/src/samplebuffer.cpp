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

#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "samplebuffer.hpp"

namespace opendlv {
namespace tool {

SampleBuffer::Iterator::Iterator(SampleBuffer const *a_outer_buffer):
    m_outer_buffer(a_outer_buffer),
    m_read_pos(0)
{
}

SampleBuffer::Iterator::~Iterator()
{
}

void SampleBuffer::Iterator::CheckOverflow(uint32_t a_data_length)
{
  if (!HasBytesLeft(a_data_length)) {
    throw std::runtime_error("Packet buffer overflow.");
  }
}

bool SampleBuffer::Iterator::HasBytesLeft(uint32_t a_data_length)
{
  uint32_t bytes_len = m_outer_buffer->GetSize();

  return (bytes_len - m_read_pos + 1) > a_data_length; 
}

bool SampleBuffer::Iterator::ReadBoolean()
{
  uint8_t byte = ReadByte();
  bool data = static_cast<bool>(byte);
  return data;
}


uint8_t SampleBuffer::Iterator::ReadByte()
{
  uint8_t const data_length = 1;
  CheckOverflow(data_length);

  uint8_t data = m_outer_buffer->m_bytes[m_read_pos];
  m_read_pos += data_length;

  return data;
}

std::shared_ptr<std::vector<uint8_t>> SampleBuffer::Iterator::ReadBytes()
{
  int16_t const data_length = ReadInteger16();
  CheckOverflow(data_length);
  
  uint32_t const start_pos = m_read_pos;
  uint32_t const end_pos = start_pos + data_length - 1;

  std::shared_ptr<std::vector<uint8_t>> data(
      new std::vector<uint8_t>(data_length));

  for (uint32_t i = start_pos, j = 0; end_pos >= i; ++i, ++j) {
    (*data)[j] = m_outer_buffer->m_bytes[i];
  }

  m_read_pos += data_length;

  return data;
}

float SampleBuffer::Iterator::ReadFloat32()
{
  uint8_t const data_length = 4;
  CheckOverflow(data_length);

  float data = 0.0;
  memcpy(&data, &m_outer_buffer->m_bytes[m_read_pos], data_length);
  m_read_pos += data_length;

  return data;
}

double SampleBuffer::Iterator::ReadFloat64()
{
  uint8_t const data_length = 8;
  CheckOverflow(data_length);

  double data = 0.0;
  memcpy(&data, &m_outer_buffer->m_bytes[m_read_pos], data_length);
  m_read_pos += data_length;

  return data;
}

int8_t SampleBuffer::Iterator::ReadInteger8()
{
  uint8_t const data_length = 1;
  CheckOverflow(data_length);

  int8_t data = 0;
  memcpy(&data, &m_outer_buffer->m_bytes[m_read_pos], data_length);
  m_read_pos += data_length;

  return data;
}

int16_t SampleBuffer::Iterator::ReadInteger16()
{
  uint8_t const data_length = 2;
  CheckOverflow(data_length);

  int16_t data = 0;
  memcpy(&data, &m_outer_buffer->m_bytes[m_read_pos], data_length);
  m_read_pos += data_length;

  return data;
}

int32_t SampleBuffer::Iterator::ReadInteger32()
{
  uint8_t const data_length = 4;
  CheckOverflow(data_length);

  int32_t data = 0;
  memcpy(&data, &m_outer_buffer->m_bytes[m_read_pos], data_length);
  m_read_pos += data_length;

  return data;
}

int64_t SampleBuffer::Iterator::ReadInteger64()
{
  uint8_t const data_length = 8;
  CheckOverflow(data_length);

  int64_t data = 0;
  memcpy(&data, &m_outer_buffer->m_bytes[m_read_pos], data_length);
  m_read_pos += data_length;

  return data;
}

std::string SampleBuffer::Iterator::ReadString()
{
  int16_t data_length = ReadInteger16();
  CheckOverflow(data_length);

  uint32_t const start_pos = m_read_pos;
  uint32_t const end_pos = start_pos + data_length - 1;

  std::stringstream string_stream;
  for (uint32_t i = start_pos; end_pos >= i; ++i) {
    string_stream << m_outer_buffer->m_bytes[i];
  }
  std::string data = string_stream.str();

  m_read_pos += data_length;

  return data;
}
void SampleBuffer::Iterator::Reset()
{
  m_read_pos = 0;
}

SampleBuffer::SampleBuffer():
    m_bytes()
{
}

SampleBuffer::SampleBuffer(std::vector<uint8_t> const &a_bytes):
    m_bytes(a_bytes)
{
}

SampleBuffer::~SampleBuffer()
{
}

void SampleBuffer::Append(std::vector<uint8_t> const &a_data)
{
  m_bytes.insert(m_bytes.end(), a_data.begin(), a_data.end());
}

void SampleBuffer::AppendBoolean(bool a_data)
{
  AppendByte(static_cast<uint8_t>(a_data));
}

void SampleBuffer::AppendByte(uint8_t a_data)
{
  std::vector<uint8_t> append(1);
  append[0] = a_data;

  Append(append);
}

void SampleBuffer::AppendBytes(std::vector<uint8_t> const &a_data)
{
  uint16_t data_length = a_data.size();
  AppendInteger16(data_length);

  Append(a_data);
}

void SampleBuffer::AppendBytesRaw(std::vector<uint8_t> const &a_data)
{
  Append(a_data);
}

void SampleBuffer::AppendFloat32(float a_data)
{
  uint8_t const data_length = 4;

  std::vector<uint8_t> append(data_length);
  memcpy(&append[0], &a_data, data_length);

  Append(append);
}

void SampleBuffer::AppendFloat64(double a_data)
{
  uint8_t const data_length = 8;

  std::vector<uint8_t> append(data_length);
  memcpy(&append[0], &a_data, data_length);

  Append(append);
}

void SampleBuffer::AppendInteger8(int8_t a_data)
{
  uint8_t const data_length = 1;

  std::vector<uint8_t> append(data_length);
  memcpy(&append[0], &a_data, data_length);

  Append(append);
}

void SampleBuffer::AppendInteger16(int16_t a_data)
{
  uint8_t const data_length = 2;

  std::vector<uint8_t> append(data_length);
  memcpy(&append[0], &a_data, data_length);

  Append(append);
}

void SampleBuffer::AppendInteger32(int32_t a_data)
{
  uint8_t const data_length = 4;

  std::vector<uint8_t> append(data_length);
  memcpy(&append[0], &a_data, data_length);

  Append(append);
}

void SampleBuffer::AppendInteger64(int64_t a_data)
{
  uint8_t const data_length = 8;

  std::vector<uint8_t> append(data_length);
  memcpy(&append[0], &a_data, data_length);

  Append(append);
}

void SampleBuffer::AppendString(std::string const &a_data)
{
  uint16_t data_length = a_data.length();
  AppendInteger16(data_length);

  AppendStringRaw(a_data);
}

void SampleBuffer::AppendStringRaw(std::string const &a_data)
{
  std::vector<uint8_t> append(a_data.begin(), a_data.end());
  Append(append);
}

std::vector<uint8_t> const &SampleBuffer::GetData() const
{
  return m_bytes;
}

std::string const SampleBuffer::GetDataString() const
{
  std::stringstream string_stream;
  for (uint32_t i = 0; i < m_bytes.size(); ++i) {
    string_stream << m_bytes[i];
  }
  std::string data = string_stream.str();

  return data;
}

std::shared_ptr<SampleBuffer::Iterator> SampleBuffer::GetIterator() const
{
  std::shared_ptr<SampleBuffer::Iterator> iterator(
      new SampleBuffer::Iterator(this));
  return iterator; 
}

uint32_t SampleBuffer::GetSize() const
{
  return m_bytes.size();
}

}
}
