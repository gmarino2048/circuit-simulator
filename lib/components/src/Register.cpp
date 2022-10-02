/**
 * @file Register.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Implementation of the Register component.
 * @version 0.1
 * @date 2022-08-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <cmath>
#include <limits>
#include <optional>
#include <string>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/StateError.hpp>
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Register.hpp>

using namespace circsim::common;
using namespace circsim::components;


Register::Register():
    _id(std::nullopt),
    _circuit(nullptr)
{
    // Use default constructors for remaining elements
}


Register::Register
(
    const uint64_t id,
    const std::string& name,
    const std::vector<uint64_t>& wire_ids
):
    _id(id),
    _name(name),
    _wire_ids(wire_ids),
    _circuit(nullptr)
{
    if( _wire_ids.size() > 64 )
    {
        throw ValueError
        (
            "Current implementation does not support registers over 64 bits"
        );
    }
}


Register::Register
(
    const uint64_t id,
    const std::string& name,
    const std::vector<uint64_t>& wire_ids,
    const Circuit& circuit
):
    _id(id),
    _name(name),
    _wire_ids(wire_ids),
    _circuit(&circuit)
{
    if( _wire_ids.size() > 64 )
    {
        throw ValueError
        (
            "Current implementation does not support registers over 64 bits"
        );
    }
}


bool Register::has_circuit() const
{
    return _circuit != nullptr;
}


void Register::set_circuit(const Circuit& circuit)
{
    _circuit = &circuit;
}


void Register::clear_circuit()
{
    _circuit = nullptr;
}


template int8_t Register::value_signed() const;
template int16_t Register::value_signed() const;
template int32_t Register::value_signed() const;
template int64_t Register::value_signed() const;

template<class T>
T Register::value_signed() const
{
    if( !has_circuit() )
    {
        throw StateError
        (
            "Cannot look up wire values without valid Circuit object"
        );
    }

    int64_t value = 0;
    for( size_t i = 0; i < _wire_ids.size(); i++ )
    {
        Wire* wire = _circuit->get<Wire>(_wire_ids[i]);
        bool wire_value = wire->high();
        if( wire_value )
        {
            uint64_t increment = uint64_t(1) << i;
            value = i < _wire_ids.size() - 1 ?
                value + increment:
                value - increment;
        }
    }

    bool out_of_bounds = value < std::numeric_limits<T>::min() ||
        value > std::numeric_limits<T>::max();

    if( out_of_bounds )
    {
        throw ValueError
        (
            (std::string) "Type " + typeid(T).name()
            + " cannot contain value " + std::to_string(value)
        );
    }

    return static_cast<T>(value);
}

template void Register::value_signed(const int8_t new_value);
template void Register::value_signed(const int16_t new_value);
template void Register::value_signed(const int32_t new_value);
template void Register::value_signed(const int64_t new_value);

template<class T>
void Register::value_signed(const T new_value)
{
    if( !has_circuit() )
    {
        throw StateError
        (
            "Cannot set register value without circuit lookup object"
        );
    }

    uint64_t regmax_unsigned = uint64_t(1) << (_wire_ids.size() - 1);

    int64_t regmax = regmax_unsigned - 1;
    int64_t regmin = regmax_unsigned * -1;

    if( new_value > regmax || new_value < regmin )
    {
        throw ValueError
        (
            "New value " + std::to_string(new_value) +
            " is out of bounds for given register size " +
            std::to_string(_wire_ids.size())
        );
    }

    bool negative = new_value < 0;
    uint64_t positive_value = negative ? new_value * -1 : new_value;

    for( size_t i = 0; i < _wire_ids.size(); i++ )
    {
        Wire* wire_ref = _circuit->get<Wire>(_wire_ids[i]);

        bool wire_value = positive_value & ( uint64_t(1) << i );
        wire_ref->set_high_low(wire_value);
    }

    if( negative )
    {
        // Invert all wire values
        for( size_t i = 0; i < _wire_ids.size(); i++ )
        {
            Wire* wire_ref = _circuit->get<Wire>(_wire_ids[i]);
            wire_ref->set_high_low(!wire_ref->high());
        }

        // Add 1
        size_t i = 0;
        bool current_carry = true;
        while( current_carry )
        {
            // We can ignore a lot of the checks for this since we've
            // checked the bounds of this number earlier in the method
            Wire* wire_ref = _circuit->get<Wire>(_wire_ids[i]);
            bool current_value = wire_ref->high();

            current_carry &= current_value;
            wire_ref->set_high_low(!current_value);
        }
    }
}


template uint8_t Register::value_unsigned() const;
template uint16_t Register::value_unsigned() const;
template uint32_t Register::value_unsigned() const;
template uint64_t Register::value_unsigned() const;

template <class T>
T Register::value_unsigned() const
{
    if( !has_circuit() )
    {
        throw StateError
        (
            "Cannot fetch register value without circuit lookup object"
        );
    }

    uint64_t value = 0;
    for( size_t i = 0; i < _wire_ids.size(); i++ )
    {
        uint64_t wire_id = _wire_ids[i];
        Wire* instance = _circuit->get<Wire>(wire_id);

        if( instance->high() )
        {
            value += uint64_t(1) << i;
        }
    }

    if( value > std::numeric_limits<T>::max() )
    {
        throw ValueError
        (
            "Register value " + std::to_string(value) +
            " is out of bounds for requested type."
        );
    }

    return static_cast<T>(value);
}


template void Register::value_unsigned(const uint8_t value);
template void Register::value_unsigned(const uint16_t value);
template void Register::value_unsigned(const uint32_t value);
template void Register::value_unsigned(const uint64_t value);

template <class T>
void Register::value_unsigned(const T value)
{
    if( !has_circuit() )
    {
        throw StateError
        (
            "Cannot set register value without a valid circuit lookup object"
        );
    }

    uint64_t max_value = std::numeric_limits<uint64_t>::max();
    if( _wire_ids.size() < (sizeof(uint64_t) * 8) )
    {
        max_value = (uint64_t(1) << _wire_ids.size()) - 1;
    }

    if( value > max_value )
    {
        throw ValueError
        (
            "Requested set value " + std::to_string(value) +
            "is greater than maximum allowed value of " +
            std::to_string(max_value)
        );
    }

    for( size_t i = 0; i < _wire_ids.size(); i++ )
    {
        Wire* wire_instance = _circuit->get<Wire>(_wire_ids[i]);
        bool wire_state = value & (uint64_t(1) << i);

        wire_instance->set_high_low(wire_state);
    }
}
