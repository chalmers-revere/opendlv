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

#ifndef TOOL_SIGNALADAPTER_SAMPLEVISITOR_HPP_
#define TOOL_SIGNALADAPTER_SAMPLEVISITOR_HPP_

#include <memory>
#include <string>

#include <opendavinci/odcore/opendavinci.h>
#include <opendavinci/odcore/base/Visitor.h>


namespace opendlv {
namespace tool {

class SampleBuffer;

class SampleVisitor
: public odcore::base::Visitor {
 public:
  SampleVisitor(std::shared_ptr<SampleBuffer>);
  SampleVisitor(SampleVisitor const &) = delete;
  SampleVisitor &operator=(SampleVisitor const &) = delete;
  virtual ~SampleVisitor();

  virtual void beginVisit(int32_t const &, std::string const &, 
      std::string const &);
  virtual void endVisit();
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, odcore::serialization::Serializable &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, bool &);
  virtual void visit(uint32_t const &, std::string const &,
      std::string const &, char &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, unsigned char &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, int8_t &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, int16_t &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, uint16_t &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, int32_t &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, uint32_t &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, int64_t &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, uint64_t &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, float &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, double &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, std::string &);
  virtual void visit(uint32_t const &, std::string const &, 
      std::string const &, void *, uint32_t const &);
  virtual void visit(const uint32_t &id, const std::string &longName, 
     const std::string &shortName, void *data, const uint32_t &count, 
     const odcore::TYPE_ &t);

 private:
  std::shared_ptr<SampleBuffer> m_sampleBuffer;
};

}
}

#endif
