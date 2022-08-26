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
    // Intentionally left blank
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
    // Intentionally left blank
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
    int64_t value = 0;

    if( !has_circuit() )
    {
        throw StateError
        (
            "Cannot look up wire values without valid Circuit object"
        );
    }

    for( size_t i = 0; i < _wire_ids.size(); i++ )
    {
        Wire* wire = _circuit->get<Wire>(_wire_ids[i]);
        bool wire_value = wire->high();
        if( wire_value )
        {
            uint64_t increment = 1 << i;
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
