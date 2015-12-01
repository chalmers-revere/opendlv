/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2008 - 2015 Christian Berger, Bernhard Rumpe
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef OPENDAVINCI_CORE_WRAPPER_CONFIGURATIONTRAITS_H_
#define OPENDAVINCI_CORE_WRAPPER_CONFIGURATIONTRAITS_H_

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * This template classes describe the configuration of a certain
         * product type.
         */
        template <typename product, product config_value> struct ConfigValue
        {
            typedef product product_type;
            static const product_type value = config_value;
        };

        template < typename product_type > struct ConfigurationTraits
        {
            typedef void configuration;
        };

        #define OPENDAVINCI_STATIC_CONFIGURATION(product, value) template <> struct ConfigurationTraits<product> { typedef ConfigValue<product, value> configuration; };
    }
}

#endif /* OPENDAVINCI_CORE_WRAPPER_CONFIGURATIONTRAITS_H_ */
