/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Header file for: opendlv.proxy.reverefh16.SteeringRequest
#ifndef OPENDLV_PROXY_REVEREFH16_STEERINGREQUEST_H_
#define OPENDLV_PROXY_REVEREFH16_STEERINGREQUEST_H_

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
				
				class SteeringRequest : public odcore::data::SerializableData, public odcore::base::Visitable {
				    private:
				        /**
				         * "Forbidden" copy constructor. Goal: The compiler should warn
				         * already at compile time for unwanted bugs caused by any misuse
				         * of the copy constructor.
				         *
				         * @param obj Reference to an object of this class.
				         */
				        SteeringRequest(const SteeringRequest &/*obj*/);
				
				        /**
				         * "Forbidden" assignment operator. Goal: The compiler should warn
				         * already at compile time for unwanted bugs caused by any misuse
				         * of the assignment operator.
				         *
				         * @param obj Reference to an object of this class.
				         * @return Reference to this instance.
				         */
				        SteeringRequest& operator=(const SteeringRequest &/*obj*/);
				
				    public:
				        SteeringRequest();
				
						SteeringRequest(double, double, double);
				
				        virtual ~SteeringRequest();
				
				        odcore::data::Container decode(const ::automotive::GenericCANMessage &gcm);
				        
				        ::automotive::GenericCANMessage encode(odcore::data::Container &c);
				        
				    	double getSteerrequestEnable_steerreq() const;
				    	void setSteerrequestEnable_steerreq(const double&);
				    	
				    	double getSteerrequestSteerreq_rwa() const;
				    	void setSteerrequestSteerreq_rwa(const double&);
				    	
				    	double getSteerrequestSteerreq_deltatrq() const;
				    	void setSteerrequestSteerreq_deltatrq(const double&);
				    	
				
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
				    	double m_steerrequestEnable_steerreq;
				    	double m_steerrequestSteerreq_rwa;
				    	double m_steerrequestSteerreq_deltatrq;
				    	
				    	std::map<uint64_t,uint64_t> m_payloads;
				    	std::vector<uint64_t> m_neededCanMessages;
				    	uint64_t m_index;
				    	
				    	::opendlv::proxy::reverefh16::SteeringRequest m_opendlv_proxy_reverefh16_SteeringRequest;
				    	
				}; // end of class "SteeringRequest"
				
			} // end of namespace "reverefh16"
		} // end of namespace "proxy"
	} // end of namespace "opendlv"
} // end of namespace canmapping

#endif /*OPENDLV_PROXY_REVEREFH16_STEERINGREQUEST_H_*/
