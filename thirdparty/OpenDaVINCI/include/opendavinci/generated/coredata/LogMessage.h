/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_LOGMESSAGE_H
#define COREDATA_LOGMESSAGE_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace coredata {
	using namespace std;
	
	class LogMessage : public core::data::SerializableData, public core::base::Visitable {
		public:
			enum LogLevel {
				NONE = 0,
				INFO = 1,
				DEBUG = 3,
				WARN = 2,
			};
		public:
			LogMessage();
	
			LogMessage(
			const LogLevel &val0, 
			const std::string &val1, 
			const std::string &val2
			);
	
			virtual ~LogMessage();
	
			/**
			 * Copy constructor.
			 *
			 * @param obj Reference to an object of this class.
			 */
			LogMessage(const LogMessage &obj);
	
			/**
			 * Assignment operator.
			 *
			 * @param obj Reference to an object of this class.
			 * @return Reference to this instance.
			 */
			LogMessage& operator=(const LogMessage &obj);
	
		public:
		public:
			/**
			 * @return logLevel.
			 */
			LogLevel getLogLevel() const;
			
			/**
			 * This method sets logLevel.
			 *
			 * @param val Value for logLevel.
			 */
			void setLogLevel(const LogLevel &val);
		public:
			/**
			 * @return logMessage.
			 */
			std::string getLogMessage() const;
			
			/**
			 * This method sets logMessage.
			 *
			 * @param val Value for logMessage.
			 */
			void setLogMessage(const std::string &val);
		public:
			/**
			 * @return componentName.
			 */
			std::string getComponentName() const;
			
			/**
			 * This method sets componentName.
			 *
			 * @param val Value for componentName.
			 */
			void setComponentName(const std::string &val);
	
		public:
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
	
		public:
			virtual void accept(core::base::Visitor &v);
	
			virtual ostream& operator<<(ostream &out) const;
			virtual istream& operator>>(istream &in);
	
			virtual const string toString() const;
	
		private:
		private:
			LogLevel m_logLevel;
		private:
			std::string m_logMessage;
		private:
			std::string m_componentName;
	
	};
} // coredata

#endif /*COREDATA_LOGMESSAGE_H*/
