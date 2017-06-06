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

#include "buffer.hpp"

namespace opendlv {
namespace logic {
namespace knowledge {

Buffer::Iterator::Iterator(Buffer const *a_outer_buffer):
    m_outer_buffer(a_outer_buffer),
    m_read_pos(0)
{
}

Buffer::Iterator::~Iterator()
{
}

void Buffer::Iterator::CheckOverflow(int a_count)
{
  if (!HasBytesLeft(a_count)) {
    throw std::runtime_error("Packet buffer overflow.");
  }
}

bool Buffer::Iterator::HasBytesLeft(int a_count)
{
  int bytes_len = m_outer_buffer->GetSize();

  return (bytes_len - m_read_pos + 1) > a_count; 
}

bool Buffer::Iterator::ReadBoolean()
{
  unsigned char byte = ReadByte();
  bool data = static_cast<bool>(byte);
  return data;
}


unsigned char Buffer::Iterator::ReadByte()
{
  int count = 1;
  CheckOverflow(count);

  unsigned char data = m_outer_buffer->m_bytes[m_read_pos];
  m_read_pos += count;

 // std::cout << " byte @" << m_read_pos-count << ": " << (int)data 
 //     << std::endl;

  return data;
}

std::shared_ptr<std::vector<unsigned char>> Buffer::Iterator::ReadBytes()
{
  long count = ReadLong();
  CheckOverflow(count);
  
  int const start_pos = m_read_pos;
  int const end_pos = start_pos + count - 1;

  std::shared_ptr<std::vector<unsigned char>> data(
      new std::vector<unsigned char>(count));

  for (int i = start_pos, j = 0; end_pos >= i; ++i, ++j) {
    (*data)[j] = m_outer_buffer->m_bytes[i];
  }

  m_read_pos += count;

//  std::cout << " bytes @" << start_pos << "-" << end_pos << ": " << data << 
//      std::endl;

  return data;
}

double Buffer::Iterator::ReadDouble()
{
  //int count = sizeof(double);
  int count = 8;
  CheckOverflow(count);

  double data = 0.0;
  if(m_itreversed)
  {
    memcpy(&data, &ReadBytesReversed(count)[0], count);
  }
  else
  {
    memcpy(&data, &m_outer_buffer->m_bytes[m_read_pos], count);
    m_read_pos += count;
  }

//  std::cout << " double @" << m_read_pos-count << "-" << m_read_pos - 1
//      << ": " << data << std::endl;

  return data;
}

float Buffer::Iterator::ReadFloat()
{
  //int count = sizeof(float);
  int count = 4;
  CheckOverflow(count);


  float data = 0.0;
  if(m_itreversed)
  {
    memcpy(&data, &ReadBytesReversed(count)[0], count);
  }
  else
  {
    memcpy(&data, &m_outer_buffer->m_bytes[m_read_pos], count);
    m_read_pos += count;
  }

//  std::cout << " float @" << m_read_pos-count << "-" << m_read_pos - 1 << ": " 
//      << data << std::endl;

  return data;
}

int Buffer::Iterator::ReadInteger()
{
  //int const count = sizeof(int);
  int const count = 4;
  CheckOverflow(count);



  int data = 0;
  if(m_itreversed)
  {
    memcpy(&data, &ReadBytesReversed(count)[0], count);
  }
  else
  {
    memcpy(&data, &m_outer_buffer->m_bytes[m_read_pos], count);
    m_read_pos += count;
  }


//  std::cout << " int @" << m_read_pos-count <<  "-" << m_read_pos - 1 << ": "
//      << data << std::endl;

  return data;
}

std::vector<unsigned char> Buffer::Iterator::ReadBytesReversed(int a_nBytes)
{
  std::vector<unsigned char> bytes(a_nBytes);
  for(int i = 0; i < a_nBytes; i++)
  {
    bytes[i] = ReadByte();
  }
  std::reverse(bytes.begin(), bytes.end());
  return bytes;
}


int Buffer::Iterator::ReadInteger24()
{
  int const count = 3;
  CheckOverflow(count);

  int data = 0;
  if(m_itreversed)
  {
    memcpy(&data, &ReadBytesReversed(count)[0], count);
  }
  else
  {
    memcpy(&data, &m_outer_buffer->m_bytes[m_read_pos], count);
    m_read_pos += count;
  }

  return data;
}

long Buffer::Iterator::ReadLong()
{
  //int const count = sizeof(long);
  int const count = 8;
  CheckOverflow(count);

  int data = 0;
  if(m_itreversed)
  {
    memcpy(&data, &ReadBytesReversed(count)[0], count);
  }
  else
  {
    memcpy(&data, &m_outer_buffer->m_bytes[m_read_pos], count);
    m_read_pos += count;
  }
  //m_read_pos += count; // TODO: strangest compiler bug, count treated as 0
  //here.

//  std::cout << " long @" << m_read_pos-count <<  "-" << m_read_pos - 1
//      << ": " << data << std::endl;

  return data;
}

short Buffer::Iterator::ReadShort()
{
  //int const count = sizeof(short);
  int const count = 2;
  CheckOverflow(count);

  short data = 0;
  if(m_itreversed)
  {
    memcpy(&data, &ReadBytesReversed(count)[0], count);
  }
  else
  {
    memcpy(&data, &m_outer_buffer->m_bytes[m_read_pos], count);
    m_read_pos += count;
  }

//  std::cout << " short @" << m_read_pos-count <<  "-" << m_read_pos - 1
//  << ": " << data << std::endl;

  return data;
}

std::string Buffer::Iterator::ReadString()
{
  int count = (int)ReadByte();
  CheckOverflow(count);

  int const start_pos = m_read_pos;
  int const end_pos = start_pos + count - 1;

  std::stringstream string_stream;
  for (int i = start_pos; end_pos >= i; ++i) {
    string_stream << m_outer_buffer->m_bytes[i];
  }
  std::string data = string_stream.str();

  m_read_pos += count;

//  std::cout << " string @" << start_pos << "-" << end_pos << ": " << data << 
//      std::endl;

  return data;
}
void Buffer::Iterator::Reset()
{
  m_read_pos = 0;

}
void Buffer::Iterator::ItReversed()
{
  if(m_itreversed){
    m_itreversed = false;
  }
  else{
    m_itreversed = true;
  }
}

Buffer::Buffer():
    m_bytes(), m_reversed(false)
{
}

Buffer::Buffer(std::vector<unsigned char> const &a_bytes):
    m_bytes(a_bytes), m_reversed(false)
{
}

Buffer::~Buffer()
{
}

void Buffer::Append(std::vector<unsigned char> const &a_bytes)
{
  if(m_reversed)
  {  
    std::vector<unsigned char> hack(a_bytes);
    std::reverse(hack.begin(), hack.end());
    m_bytes.insert(m_bytes.end(), hack.begin(), hack.end());
  }
  else{
    m_bytes.insert(m_bytes.end(), a_bytes.begin(), a_bytes.end());
  }
}

void Buffer::AppendBoolean(bool a_bool)
{
  AppendByte(static_cast<unsigned char>(a_bool));
}

void Buffer::AppendByte(unsigned char a_byte)
{
  std::vector<unsigned char> append(1);
  append[0] = a_byte;

  Append(append);
}

void Buffer::AppendBytes(std::vector<unsigned char> const &a_bytes)
{
  long len = a_bytes.size();
  AppendLong(len);

  Append(a_bytes);
}

void Buffer::AppendBytesRaw(std::vector<unsigned char> const &a_bytes)
{
  Append(a_bytes);
}

void Buffer::AppendDouble(double a_double)
{
  //int count = sizeof(double);
  int count = 8;

  std::vector<unsigned char> append(count);
  memcpy(&append[0], &a_double, count);

  Append(append);
}

void Buffer::AppendFloat(float a_float)
{
  //int count = sizeof(dloat);
  int count = 4;

  std::vector<unsigned char> append(count);
  memcpy(&append[0], &a_float, count);

  Append(append);
}

void Buffer::AppendInteger(int a_integer)
{
  //int count = sizeof(int);
  int count = 4;

  std::vector<unsigned char> append(count);
  memcpy(&append[0], &a_integer, count);

  Append(append);
}

void Buffer::AppendLong(long a_long)
{
  //int count = sizeof(long);
  int count = 8;

  std::vector<unsigned char> append(count);
  memcpy(&append[0], &a_long, count);

  Append(append);
}

void Buffer::AppendShort(short a_short)
{
  //int count = sizeof(short);
  int count = 2;

  std::vector<unsigned char> append(count);
  memcpy(&append[0], &a_short, count);

  Append(append);
}

void Buffer::AppendString(std::string const &a_string)
{
  int len = a_string.length();
  if (len > 255) {
    throw std::runtime_error("String is too long (> 255).");
  }
  AppendByte((unsigned char)len);

  std::vector<unsigned char> append(a_string.begin(), a_string.end());
  Append(append);
}

void Buffer::AppendStringRaw(std::string const &a_string)
{
  std::vector<unsigned char> append(a_string.begin(), a_string.end());
  Append(append);
}

std::vector<unsigned char> const &Buffer::GetBytes() const
{
  return m_bytes;
}

std::shared_ptr<Buffer::Iterator> Buffer::GetIterator() const
{
  std::shared_ptr<Buffer::Iterator> iterator(new Buffer::Iterator(this));
  return iterator; 
}

int Buffer::GetSize() const
{
  return m_bytes.size();
}

unsigned char Buffer::ReadByte(int a_index) const
{
  return m_bytes[a_index];
}
void Buffer::Reversed(){
  if(m_reversed){
    m_reversed = false;
  }
  else{
    m_reversed = true;
  }
}

}
}
}
