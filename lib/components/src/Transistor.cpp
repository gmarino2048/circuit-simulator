/**
 * @file Transistor.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief This file contains the class definitions for the
 *        Transistor object.
 * @version 0.1
 * @date 2021-10-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// C++ Stdlib Includes
#include <cstdint>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>

// Library Includes
// (none)

// Project Includes
#include <circsim/components/Wire.hpp>
#include <circsim/components/Transistor.hpp>

using namespace circsim::components;
using WireState = circsim::components::Wire::State;


static constexpr uint8_t HIGH_STATES = 
    WireState::HIGH | WireState::PULLED_HIGH | WireState::FLOATING_HIGH;


bool Transistor::_update_nmos(const WireState gate_state)
{
    State old_state = current_state();
    _current_state = gate_state & HIGH_STATES ? ON : OFF;

    return current_state() != old_state;
}


bool Transistor::_update_pmos(const WireState gate_state)
{
    State old_state = current_state();
    _current_state = gate_state & HIGH_STATES ? OFF : ON;

    return current_state() != old_state;
}


Transistor::Transistor(): _id(std::nullopt),
                          _type(NMOS),
                          _gate_id(0),
                          _source_id(0),
                          _drain_id(0),
                          _current_state(OFF)
{
    // Default constructor for all other members
}


Transistor::Transistor
(
    const uint64_t id,
    const uint64_t gate_id,
    const uint64_t source_id,
    const uint64_t drain_id,
    const Type type
):  _id(id),
    _type(type),
    _gate_id(gate_id),
    _source_id(source_id),
    _drain_id(drain_id),
    _current_state(OFF)
{
    // Default constructor for all other members
}


Transistor::Transistor
(
    const uint64_t id,
    const std::string &name,
    const uint64_t gate_id,
    const uint64_t source_id,
    const uint64_t drain_id,
    const Type type
):  _id(id),
    _name(name),
    _type(type),
    _gate_id(gate_id),
    _source_id(source_id),
    _drain_id(drain_id),
    _current_state(OFF)
{
    // No other members to initialize
}


bool Transistor::update_state(const WireState gate_state)
{
    switch( this->_type )
    {
        case NMOS:
            return this->_update_nmos(gate_state);
        case PMOS:
            return this->_update_pmos(gate_state);
        default:
            throw std::runtime_error
            (
                "Unrecognized transistor type: " +
                std::to_string(this->_type)
            );
    }
}


bool Transistor::operator==(const Transistor& rhs) const
{
    bool equivalent = _id == rhs._id;

    equivalent &= _name == rhs._name;

    equivalent &= _source_id == rhs._source_id;
    equivalent &= _gate_id == rhs._gate_id;
    equivalent &= _drain_id == rhs._drain_id;

    return equivalent;
}


Transistor::operator std::string() const
{
    std::stringstream ss;

    ss << "Transistor \"" << this->_name << "\":\n";

    ss << "\tId:\t\t";
    if( this->_id.has_value() )
    {
        ss << "0x" << std::uppercase << std::setfill('0') << std::setw(16)
            << this->_id.value();
    }
    else
    {
        ss << "None";
    }
    ss << "\n";

    ss << "\tGate:\t\t" << std::uppercase << std::setfill('0') << std::setw(16)
        << this->_gate_id << "\n";

    ss << "\tSource:\t\t" << std::uppercase << std::setfill('0') << std::setw(16)
        << this->_source_id << "\n";

    ss << "\tDrain:\t\t" << std::uppercase << std::setfill('0') << std::setw(16)
        << this->_drain_id << "\n";

    return ss.str();
}
