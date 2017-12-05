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

#include <iostream>
#include <vector>

#include <opendavinci/odcore/serialization/Serializable.h>
#include <opendavinci/odcore/data/SerializableData.h>

#include "samplebuffer.hpp"
#include "samplevisitor.hpp"

namespace opendlv {
namespace tool {


SampleVisitor::SampleVisitor(std::shared_ptr<SampleBuffer> a_sampleBuffer)
  : odcore::base::Visitor(),
  m_sampleBuffer(a_sampleBuffer)
{
}

SampleVisitor::~SampleVisitor()
{
}

void SampleVisitor::beginVisit(const int32_t &, std::string const &, 
    std::string const &) 
{
}

void SampleVisitor::endVisit() 
{
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, odcore::serialization::Serializable &) 
{
  try {
 //   odcore::data::SerializableData& tmp = 
 //       dynamic_cast<odcore::data::SerializableData&>(a_v);
 //   m_sampleBuffer->AppendString(tmp.toString());
  } catch(...) {
  }
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, bool &a_v)
{
  m_sampleBuffer->AppendBoolean(a_v);
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, char &a_v)
{
  m_sampleBuffer->AppendInteger8(static_cast<int8_t>(a_v));
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, unsigned char &a_v)
{
  m_sampleBuffer->AppendByte(static_cast<uint8_t>(a_v));
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, int8_t &a_v)
{
  m_sampleBuffer->AppendInteger8(a_v);
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, int16_t &a_v)
{
  m_sampleBuffer->AppendInteger16(a_v);
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, uint16_t &a_v)
{
  m_sampleBuffer->AppendInteger16(a_v);
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, int32_t &a_v)
{
  m_sampleBuffer->AppendInteger32(a_v);
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, uint32_t &a_v)
{
  m_sampleBuffer->AppendInteger32(a_v);
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, int64_t &a_v)
{
  m_sampleBuffer->AppendInteger64(a_v);
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, uint64_t &a_v)
{
  m_sampleBuffer->AppendInteger64(a_v);
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, float &a_v)
{
  m_sampleBuffer->AppendFloat32(a_v);
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, double &a_v)
{
  m_sampleBuffer->AppendFloat64(a_v);
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, std::string &a_v)
{
  m_sampleBuffer->AppendString(a_v);
}

void SampleVisitor::visit(uint32_t const &, std::string const &, 
    std::string const &, void *a_data, uint32_t const &a_size)
{
  char *p = reinterpret_cast<char*>(a_data);

  std::vector<uint8_t> dataVec;
  for (uint16_t i = 0; i < a_size; ++i) {
    dataVec.push_back(p[i]);
  }
  m_sampleBuffer->AppendBytes(dataVec);
}

void SampleVisitor::visit(const uint32_t &/*id*/, const std::string &/*longName*/, const std::string &/*shortName*/, void */*data*/, const uint32_t &/*count*/, const odcore::TYPE_ &/*t*/) {
  std::cerr << "SampleVisitor::visit not implemented." << std::endl;
}


}
}
