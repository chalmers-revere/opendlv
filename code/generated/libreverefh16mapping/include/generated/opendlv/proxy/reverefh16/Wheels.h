/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Header file for: opendlv.proxy.reverefh16.Wheels
#ifndef OPENDLV_PROXY_REVEREFH16_WHEELS_H_
#define OPENDLV_PROXY_REVEREFH16_WHEELS_H_

#include <memory>
#include <cmath>
#include <vector>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/base/Visitable.h>
#include <opendavinci/odcore/data/SerializableData.h>

#include <automotivedata/GeneratedHeaders_AutomotiveData.h>
#include <opendlvdata/GeneratedHeaders_opendlvdata.h>

namespace canmapping {
	namespace opendlv {
		namespace proxy {
			namespace reverefh16 {
				using namespace std;
				
				class Wheels : public odcore::data::SerializableData, public odcore::base::Visitable {
				    private:
				        /**
				         * "Forbidden" copy constructor. Goal: The compiler should warn
				         * already at compile time for unwanted bugs caused by any misuse
				         * of the copy constructor.
				         *
				         * @param obj Reference to an object of this class.
				         */
				        Wheels(const Wheels &/*obj*/);
				
				        /**
				         * "Forbidden" assignment operator. Goal: The compiler should warn
				         * already at compile time for unwanted bugs caused by any misuse
				         * of the assignment operator.
				         *
				         * @param obj Reference to an object of this class.
				         * @return Reference to this instance.
				         */
				        Wheels& operator=(const Wheels &/*obj*/);
				
				    public:
				        Wheels();
				
						Wheels(double, double, double, double, double, double);
				
				        virtual ~Wheels();
				
				        odcore::data::Container decode(const ::automotive::GenericCANMessage &gcm);
				        
				        ::automotive::GenericCANMessage encode(odcore::data::Container &c);
				        
				    	double getWheelspeeds1Frontaxle1wheelspeedleft() const;
				    	void setWheelspeeds1Frontaxle1wheelspeedleft(const double&);
				    	
				    	double getWheelspeeds1Frontaxle1wheelspeedright() const;
				    	void setWheelspeeds1Frontaxle1wheelspeedright(const double&);
				    	
				    	double getWheelspeeds1Driveaxle1wheelspeedleft() const;
				    	void setWheelspeeds1Driveaxle1wheelspeedleft(const double&);
				    	
				    	double getWheelspeeds1Driveaxle1wheelspeedright() const;
				    	void setWheelspeeds1Driveaxle1wheelspeedright(const double&);
				    	
				    	double getWheelspeeds2Driveaxle2wheelspeedleft() const;
				    	void setWheelspeeds2Driveaxle2wheelspeedleft(const double&);
				    	
				    	double getWheelspeeds2Driveaxle2wheelspeedright() const;
				    	void setWheelspeeds2Driveaxle2wheelspeedright(const double&);
				    	
				
				        virtual const string toString() const;
				
						virtual ostream& operator<<(ostream &out) const;
				
						virtual istream& operator>>(istream &in);
				
						virtual void accept(odcore::base::Visitor &v);
						
				
				        /**
				         * This method returns the message id.
				         *
				         * @return Message id.
				         */
				        static int32_t ID();
				
				        /**
				         * This method returns the short message name.
				         *
				         * @return Short message name.
				         */
				        static const string ShortName();
				
				        /**
				         * This method returns the long message name include package/sub structure.
				         *
				         * @return Long message name.
				         */
				        static const string LongName();
				
				        /**
				         * This method returns the message id.
				         *
				         * @return Message id.
				         */
				        virtual int32_t getID() const;
				
				        /**
				         * This method returns the short message name.
				         *
				         * @return Short message name.
				         */
				        virtual const string getShortName() const;
				
				        /**
				         * This method returns the long message name include package/sub structure.
				         *
				         * @return Long message name.
				         */
				        virtual const string getLongName() const;
				    private:
				    	double m_wheelspeeds1Frontaxle1wheelspeedleft;
				    	double m_wheelspeeds1Frontaxle1wheelspeedright;
				    	double m_wheelspeeds1Driveaxle1wheelspeedleft;
				    	double m_wheelspeeds1Driveaxle1wheelspeedright;
				    	double m_wheelspeeds2Driveaxle2wheelspeedleft;
				    	double m_wheelspeeds2Driveaxle2wheelspeedright;
				    	
				    	std::map<uint64_t,uint64_t> m_payloads;
				    	std::vector<uint64_t> m_neededCanMessages;
				    	uint64_t m_index;
				    	
				    	::opendlv::proxy::reverefh16::Wheels m_opendlv_proxy_reverefh16_Wheels;
				    	
				}; // end of class "Wheels"
				
			} // end of namespace "reverefh16"
		} // end of namespace "proxy"
	} // end of namespace "opendlv"
} // end of namespace canmapping

#endif /*OPENDLV_PROXY_REVEREFH16_WHEELS_H_*/
