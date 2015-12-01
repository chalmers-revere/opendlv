/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef AUTOMOTIVE_MINIATURE_SENSORBOARDDATA_H
#define AUTOMOTIVE_MINIATURE_SENSORBOARDDATA_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include <map>

#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace automotive {
	namespace miniature {
		using namespace std;
		
		class SensorBoardData : public core::data::SerializableData, public core::base::Visitable {
			public:
				SensorBoardData();
		
				SensorBoardData(
				const uint32_t &val0, 
				const map<uint32_t, double> &val1
				);
		
				virtual ~SensorBoardData();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				SensorBoardData(const SensorBoardData &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				SensorBoardData& operator=(const SensorBoardData &obj);
		
			public:
				/**
				 * @return numberOfSensors.
				 */
				uint32_t getNumberOfSensors() const;
				
				/**
				 * This method sets numberOfSensors.
				 *
				 * @param val Value for numberOfSensors.
				 */
				void setNumberOfSensors(const uint32_t &val);
			public:
				/**
				 * @return distances.
				 */
				std::map<uint32_t, double> getMapOfDistances() const;
			
				/**
				 * This method sets distances.
				 *
				 * @param val Value for distances.
				 */
				void setMapOfDistances(const std::map<uint32_t, double> &val);
			
				/**
				 * This method clears the map of distances.
				 */
				void clear_MapOfDistances();
			
				/**
				 * @return the size of the map of distances.
				 */
				uint32_t getSize_MapOfDistances() const;
			
				/**
				 * @return true if the map of distances is empty.
				 */
				bool isEmpty_MapOfDistances() const;
			
				/**
				 * This method adds an element to the map of distances.
				 *
				 * @param key Key of the key/value pair to be added to the map of distances.
				 * @param val Value of the key/value pair to be added to the map of distances.
				 */
				void putTo_MapOfDistances(const uint32_t &key, const double &val);
			
				/**
				 * @return true if the map of distances is contains the given key.
				 */
				bool containsKey_MapOfDistances(const uint32_t &key) const;
			
				/**
				 * This method returns the value to the given key. Make sure to check if the given key exists otherwise a new key/value pair will be added (cf. C++ standard).
				 *
				 * @param key Key for which the value should be returned from the map of distances.
				 * @return Value to given key from the map of distances is contains the given key.
				 */
				double getValueForKey_MapOfDistances(const uint32_t &key);
			
				/**
				 * @return Pair of iterators for the begin and end of the map of distances.
				 */
				std::pair<std::map<uint32_t, double>::iterator, std::map<uint32_t, double>::iterator> iteratorPair_MapOfDistances();
		
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
				uint32_t m_numberOfSensors;
			private:
				std::map<uint32_t, double> m_mapOfDistances;
		
		};
	} // miniature
} // automotive

#endif /*AUTOMOTIVE_MINIATURE_SENSORBOARDDATA_H*/
