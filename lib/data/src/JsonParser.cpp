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
#include <filesystem>
#include <fstream>
#include <optional>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <vector>

// Library Includes
#include <boost/json.hpp>

// Project Includes
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Circuit.hpp>
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
std::vector<uint64_t> JsonParser::_convert_object(const boost::json::value& value) try
{
    boost::json::array value_array = value.as_array();

    std::vector<uint64_t> num_values;
    num_values.reserve(value_array.size());

    for( const boost::json::value& arr_value : value_array )
    {
        num_values.push_back
        (
            _convert_object<uint64_t>(arr_value)
        );
    }

    return num_values;
}
catch( const std::invalid_argument& ex )
{
    throw ValueError
    (
        (std::string) "Cannot convert JSON value to vector<uint64_t>: " + ex.what()
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
std::vector<std::string> JsonParser::_convert_object(const boost::json::value& value) try
{
    boost::json::array value_array = value.as_array();

    std::vector<std::string> string_array;
    string_array.reserve(value_array.size());

    for( const boost::json::value& arr_value : value_array )
    {
        string_array.push_back
        (
            _convert_object<std::string>(arr_value)
        );
    }

    return string_array;
}
catch( const std::invalid_argument& ex )
{
    throw ValueError
    (
        (std::string) "Cannot convert JSON value to vector<string>: " + ex.what()
    );
}


template<>
Transistor::Type JsonParser::_convert_object(const boost::json::value& value) try
{
    std::string type = _convert_object<std::string>(value);

    if( type == TRANSISTOR_TYPE_NMOS )
    {
        return Transistor::Type::NMOS;
    }
    else if( type == TRANSISTOR_TYPE_PMOS )
    {
        return Transistor::Type::PMOS;
    }
    else
    {
        throw std::invalid_argument("Must be one of { \"NMOS\", \"PMOS\" }");
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

    if( name.has_value() )
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


template<>
std::vector<Transistor> JsonParser::_convert_object(const boost::json::value& value) try
{
    boost::json::array value_array = value.as_array();

    std::vector<Transistor> values;
    values.reserve(value_array.size());

    for( const boost::json::value& arr_value : value_array )
    {
        values.push_back
        (
            _convert_object<Transistor>(arr_value)
        );
    }

    return values;
}
catch( const std::invalid_argument& ex )
{
    throw ValueError
    (
        (std::string) "Cannot convert JSON value to vector<Transistor>: " + ex.what()
    );
}


template<>
Wire::PulledStatus JsonParser::_convert_object(const boost::json::value& value) try
{
    std::string status = _convert_object<std::string>(value);

    if( status == WIRE_PULLED_HIGH )
    {
        return Wire::PulledStatus::PS_HIGH;
    }
    else if( status == WIRE_PULLED_LOW )
    {
        return Wire::PulledStatus::PS_LOW;
    }
    else if( status == WIRE_PULLED_NONE )
    {
        return Wire::PulledStatus::PS_NONE;
    }
    else
    {
        throw std::invalid_argument("Must be one of { \"HIGH\", \"LOW\", \"NONE\" }");
    }
}
catch( const std::invalid_argument& ex )
{
    throw ValueError
    (
        (std::string) "Cannot convert JSON value to Wire::PulledStatus: " + ex.what()
    );
}


template<>
Wire::SpecialWireType JsonParser::_convert_object(const boost::json::value& value) try
{
    std::string special = _convert_object<std::string>(value);

    if( special == WIRE_SPECIAL_VCC )
    {
        return Wire::SpecialWireType::SW_VCC;
    }
    else if( special == WIRE_SPECIAL_GND )
    {
        return Wire::SpecialWireType::SW_GND;
    }
    else if( special == WIRE_SPECIAL_NONE )
    {
        return Wire::SpecialWireType::SW_NONE;
    }
    else
    {
        throw std::invalid_argument("Must be one of { \"VCC\", \"GND\", \"NONE\" }");
    }
}
catch( const std::invalid_argument& ex )
{
    throw ValueError
    (
        (std::string) "Cannot convert JSON value to Wire::SpecialWireType: " + ex.what()
    );
}


template<>
Wire JsonParser::_convert_object(const boost::json::value& value) try
{
    boost::json::object wire_object = value.as_object();

    uint8_t id = _convert_object<uint64_t>
    (
        wire_object.at(WIRE_VALUE_ID)
    );

    std::optional<std::string> name = std::nullopt;
    if( boost::json::value* name_ptr = wire_object.if_contains(WIRE_VALUE_NAME) )
    {
        name = _convert_object<std::string>(*name_ptr);
    }

    std::optional<std::vector<std::string>> alt_names = std::nullopt;
    if( boost::json::value* alt_ptr = wire_object.if_contains(WIRE_VALUE_ALT) )
    {
        alt_names = _convert_object<std::vector<std::string>>(*alt_ptr);
    }

    Wire::PulledStatus pulled = Wire::PulledStatus::PS_NONE;
    if( boost::json::value* pulled_ptr = wire_object.if_contains(WIRE_VALUE_PULLED) )
    {
        pulled = _convert_object<Wire::PulledStatus>(*pulled_ptr);
    }

    Wire::SpecialWireType special = Wire::SpecialWireType::SW_NONE;
    if( boost::json::value* special_ptr = wire_object.if_contains(WIRE_VALUE_SPECIAL) )
    {
        special = _convert_object<Wire::SpecialWireType>(*special_ptr);
    }

    std::vector<uint64_t> ctrl_transistors = _convert_object<std::vector<uint64_t>>
    (
        wire_object.at(WIRE_VALUE_CTRL)
    );

    std::vector<uint64_t> gate_transistors = _convert_object<std::vector<uint64_t>>
    (
        wire_object.at(WIRE_VALUE_GATE)
    );

    Wire wire;
    if( special == Wire::SpecialWireType::SW_NONE )
    {
        if( name.has_value() )
        {
            wire = Wire(id, name.value(), pulled, ctrl_transistors, gate_transistors);
        }
        else
        {
            wire = Wire(id, "", pulled, ctrl_transistors, gate_transistors);
        }
    }
    else
    {
        wire = Wire(id, special, ctrl_transistors, gate_transistors);
    }

    if( alt_names.has_value() )
    {
        for( const std::string& alt_name : alt_names.value() )
        {
            wire.add_name(alt_name);
        }
    }

    return wire;
}
catch( const std::exception& ex )
{
    throw ValueError
    (
        (std::string) "Cannot convert JSON value to Wire: " + ex.what()
    );
}


template<>
std::vector<Wire> JsonParser::_convert_object(const boost::json::value& value) try
{
    boost::json::array value_array = value.as_array();

    std::vector<Wire> values;
    values.reserve(value_array.size());

    for( const boost::json::value& arr_value : value_array )
    {
        values.push_back
        (
            _convert_object<Wire>(arr_value)
        );
    }

    return values;
}
catch( const std::invalid_argument& ex )
{
    throw ValueError
    (
        (std::string) "Cannot convert JSON value to vector<Wire>: " + ex.what()
    );
}


template<>
Circuit JsonParser::_convert_object(const boost::json::value& value) try
{
    boost::json::object circuit_object = value.as_object();

    std::optional<std::string> name = std::nullopt;
    if( boost::json::value* name_ptr = circuit_object.if_contains(CIRCUIT_VALUE_NAME) )
    {
        name = _convert_object<std::string>(*name_ptr);
    }

    std::vector<Transistor> transistors = _convert_object<std::vector<Transistor>>
    (
        circuit_object.at(CIRCUIT_VALUE_TRANSISTORS)
    );

    std::vector<Wire> wires = _convert_object<std::vector<Wire>>
    (
        circuit_object.at(CIRCUIT_VALUE_WIRES)
    );

    Circuit circuit;
    if( name.has_value() )
    {
        circuit = Circuit(name.value());
    }

    circuit.add_all_components(transistors);
    circuit.add_all_components(wires);

    return circuit;
}
catch( const std::invalid_argument& ex )
{
    throw ValueError
    (
        (std::string) "Could not convert JSON object to Circuit: " + ex.what()
    );
}


Circuit JsonParser::parse(const std::string& json_string)
{
    boost::json::parse_options opts;
    opts.allow_comments = true;
    opts.allow_trailing_commas = true;
    opts.allow_invalid_utf8 = false;

    boost::json::monotonic_resource resource;
    const boost::json::value circuit_value = boost::json::parse
    (
        json_string,
        &resource,
        opts
    );

    return _convert_object<Circuit>(circuit_value);
}


Circuit JsonParser::parse(const std::filesystem::path& json_filepath)
{
    std::string contents;

    {
        std::ifstream input_file(json_filepath);
        contents = std::string
        (
            std::istreambuf_iterator<std::string::value_type>(input_file),
            std::istreambuf_iterator<std::string::value_type>()
        );
    }

    return parse(contents);
}
