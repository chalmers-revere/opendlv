/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#include <memory>

#include "opendavinci/odcore/base/Hash.h"
#include "opendavinci/odcore/base/Deserializer.h"
#include "opendavinci/odcore/base/SerializationFactory.h"
#include "opendavinci/odcore/base/Serializer.h"


#include "opendlvdata/generated/opendlv/action/Correction.h"

namespace opendlv {
	namespace action {
			using namespace std;
			using namespace odcore::base;
		
		
			Correction::Correction() :
			    SerializableData(), Visitable()
				, m_startTime()
				, m_type("")
				, m_isInhibitory(false)
				, m_amplitude(0)
			{
			}
		
			Correction::Correction(
				const odcore::data::TimeStamp &val0, 
				const std::string &val1, 
				const bool &val2, 
				const float &val3
			) :
			    SerializableData(), Visitable()
				, m_startTime(val0)
				, m_type(val1)
				, m_isInhibitory(val2)
				, m_amplitude(val3)
			{
			}
		
			Correction::Correction(const Correction &obj) :
			    SerializableData(), Visitable()
				, m_startTime(obj.m_startTime)
				, m_type(obj.m_type)
				, m_isInhibitory(obj.m_isInhibitory)
				, m_amplitude(obj.m_amplitude)
			{
			}
			
			Correction::~Correction() {
			}
		
			Correction& Correction::operator=(const Correction &obj) {
				m_startTime = obj.m_startTime;
				m_type = obj.m_type;
				m_isInhibitory = obj.m_isInhibitory;
				m_amplitude = obj.m_amplitude;
				return (*this);
			}
		
			int32_t Correction::ID() {
				return 190;
			}
		
			const string Correction::ShortName() {
				return "Correction";
			}
		
			const string Correction::LongName() {
				return "opendlv.action.Correction";
			}
		
			int32_t Correction::getID() const {
				return Correction::ID();
			}
		
			const string Correction::getShortName() const {
				return Correction::ShortName();
			}
		
			const string Correction::getLongName() const {
				return Correction::LongName();
			}
		
			odcore::data::TimeStamp Correction::getStartTime() const {
				return m_startTime;
			}
			
			void Correction::setStartTime(const odcore::data::TimeStamp &val) {
				m_startTime = val;
			}
			std::string Correction::getType() const {
				return m_type;
			}
			
			void Correction::setType(const std::string &val) {
				m_type = val;
			}
			bool Correction::getIsInhibitory() const {
				return m_isInhibitory;
			}
			
			void Correction::setIsInhibitory(const bool &val) {
				m_isInhibitory = val;
			}
			float Correction::getAmplitude() const {
				return m_amplitude;
			}
			
			void Correction::setAmplitude(const float &val) {
				m_amplitude = val;
			}
		
			void Correction::accept(odcore::base::Visitor &v) {
				v.beginVisit();
				v.visit(CRC32 < CharList<'s', CharList<'t', CharList<'a', CharList<'r', CharList<'t', CharList<'T', CharList<'i', CharList<'m', CharList<'e', NullType> > > > > > > > >  >::RESULT, 1, "Correction.startTime", "startTime", m_startTime);
				v.visit(CRC32 < CharList<'t', CharList<'y', CharList<'p', CharList<'e', NullType> > > >  >::RESULT, 2, "Correction.type", "type", m_type);
				v.visit(CRC32 < CharList<'i', CharList<'s', CharList<'I', CharList<'n', CharList<'h', CharList<'i', CharList<'b', CharList<'i', CharList<'t', CharList<'o', CharList<'r', CharList<'y', NullType> > > > > > > > > > > >  >::RESULT, 3, "Correction.isInhibitory", "isInhibitory", m_isInhibitory);
				v.visit(CRC32 < CharList<'a', CharList<'m', CharList<'p', CharList<'l', CharList<'i', CharList<'t', CharList<'u', CharList<'d', CharList<'e', NullType> > > > > > > > >  >::RESULT, 4, "Correction.amplitude", "amplitude", m_amplitude);
				v.endVisit();
			}
		
			const string Correction::toString() const {
				stringstream s;
		
		
				s << "StartTime: " << getStartTime().toString() << " ";
				s << "Type: " << getType() << " ";
				s << "IsInhibitory: " << getIsInhibitory() << " ";
				s << "Amplitude: " << getAmplitude() << " ";
		
				return s.str();
			}
		
			ostream& Correction::operator<<(ostream &out) const {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Serializer> s = sf.getSerializer(out);
		
				s->write(1,
						m_startTime);
				s->write(2,
						m_type);
				s->write(3,
						m_isInhibitory);
				s->write(4,
						m_amplitude);
				return out;
			}
		
			istream& Correction::operator>>(istream &in) {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
		
				d->read(1,
						m_startTime);
				d->read(2,
						m_type);
				d->read(3,
						m_isInhibitory);
				d->read(4,
						m_amplitude);
				return in;
			}
	} // action
} // opendlv
