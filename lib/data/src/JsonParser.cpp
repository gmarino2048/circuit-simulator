/**
 * @file JsonParser.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Implementation for the JsonParser class
 * @version 0.1
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <cinttypes>
#include <optional>
#include <stdexcept>
#include <string>

// Library Includes
#include <boost/json.hpp>

// Project Includes
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>
#include <circsim/data/JsonConstants.hpp>
#include <circsim/data/JsonParser.hpp>

using namespace circsim::common;
using namespace circsim::components;
using namespace circsim::data;

template<>
uint64_t JsonParser::_convert_object(const boost::json::value& value) try
{
   return value.as_uint64();
}
catch( const std::invalid_argument& ex )
{
    throw ValueError
    (
        (std::string) "Cannot convert JSON value to uint64_t: " + ex.what()
    );
}


template<>
std::string JsonParser::_convert_object(const boost::json::value& value) try
{
    return static_cast<std::string>(value.as_string());
}
catch( const std::invalid_argument& ex )
{
    throw ValueError
    (
        (std::string) "Cannot convert JSON value to string: " + ex.what()
    );
}


template<>
Transistor::Type JsonParser::_convert_object(const boost::json::value& value) try
{
    uint64_t type_value = value.as_uint64();

    switch( type_value )
    {
        case Transistor::Type::NMOS:
            return Transistor::Type::NMOS;
        case Transistor::Type::PMOS:
            return Transistor::Type::PMOS;
        default:
            throw std::invalid_argument("Value out of range for Transistor::Type");
    }
}
catch( const std::invalid_argument& ex )
{
    throw ValueError
    (
        (std::string) "Cannot convert JSON value to Transistor::Type: " + ex.what()
    );
}


template<>
Transistor JsonParser::_convert_object(const boost::json::value& value) try
{
    boost::json::object transistor_object = value.as_object();

    std::optional<std::string> name = std::nullopt;
    if( const boost::json::value* name_value = transistor_object.if_contains(TRANSISTOR_VALUE_NAME) )
    {
        name = _convert_object<std::string>(*name_value);
    }

    Transistor::Type type = _convert_object<Transistor::Type>
    (
        transistor_object.at(TRANSISTOR_VALUE_TYPE)
    );

    const size_t ID_COUNT = 4;
    uint64_t ids[ID_COUNT];
    const char* keys[ID_COUNT] =
    {
        TRANSISTOR_VALUE_ID,
        TRANSISTOR_VALUE_GATE,
        TRANSISTOR_VALUE_SOURCE,
        TRANSISTOR_VALUE_DRAIN
    };

    for(size_t i = 0; i < ID_COUNT; i++)
    {
        ids[i] = _convert_object<uint64_t>
        (
            transistor_object.at(keys[i])
        );
    }

    if( name )
    {
        return Transistor(ids[0], name.value(), ids[1], ids[2], ids[3], type);
    }
    else
    {
        return Transistor(ids[0], ids[1], ids[2], ids[3], type);
    }
}
catch( const std::exception& ex )
{
    throw ValueError
    (
        (std::string) "Cannot convert JSON value to Transistor: " + ex.what()
    );
}
