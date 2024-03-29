/**
 * @file Wire.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief This file contains the definitions for the wire component.
 * @version 0.1
 * @date 2022-03-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <cstdint>
#include <optional>
#include <iomanip>
#include <sstream>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/StateError.hpp>
#include <circsim/components/Wire.hpp>

using namespace circsim::components;

// Local constants
const char *ST_UNK = "UNKNOWN";
const char *ST_GND = "GROUNDED";
const char *ST_PLL = "PULLED LOW";
const char *ST_FLL = "FLOATING LOW";
const char *ST_FLT = "FLOATING";
const char *ST_FLH = "FLOATING HIGH";
const char *ST_PLH = "PULLED HIGH";
const char *ST_HGH = "HIGH";

const char *WIRE_VCC = "VCC";
const char *WIRE_GND = "GND";


/**
 * Default constructor for the wire object. Make sure that the
 * ID of the wire is -1 so the simulator can quickly detect an error
 */
Wire::Wire(): _id(std::nullopt),
              _pulled(PS_NONE),
              _special_type(SW_NONE),
              _state(FLOATING)
{
    // All other fields get default constructor calls
}


/**
 * Construct a special wire object given an ID and a specific type
 */
Wire::Wire
(
    const uint64_t id,
    const SpecialWireType special_type,
    const std::vector<uint64_t> &control_transistors,
    const std::vector<uint64_t> &gate_transistors
):  _id(id),
    _pulled(PS_NONE),
    _special_type(special_type),
    _trans_ctl_ids(control_transistors),
    _trans_gate_ids(gate_transistors)
{
    _primary_name = special_wire_name(special_type);
}


/**
 * Constructs a new standard wire which is internally driven
 */
Wire::Wire
(
    const uint64_t id,
    const std::string &name,
    const PulledStatus pulled,
    const std::vector<uint64_t> &control_transistors,
    const std::vector<uint64_t> &gate_transistors
):  _id(id),
    _primary_name(name),
    _pulled(pulled),
    _special_type(SW_NONE),
    _trans_ctl_ids(control_transistors),
    _trans_gate_ids(gate_transistors)
{
    set_floating();
}


bool Wire::special() const
{
    return _special_type != SW_NONE;
}


void Wire::add_name(const std::string &new_name)
{
    this->_other_names.push_back(new_name);
}


bool Wire::pulled() const noexcept
{
    switch ( pulled_state() )
    {
        case PS_HIGH:       // Fallthrough
        case PS_LOW:
            return true;

        default:
            return false;
    }
}


bool Wire::low() const noexcept
{
    static const uint8_t LOW_STATES = GROUNDED | PULLED_LOW | FLOATING_LOW;
    return (bool)(this->_state & LOW_STATES);
}


bool Wire::high() const noexcept
{
    static const uint8_t HIGH_STATES = HIGH | PULLED_HIGH | FLOATING_HIGH;
    return (bool)(this->_state & HIGH_STATES);
}


void Wire::set_floating()
{
    // If the wire is VCC or GND, DON'T update the state
    if ( special() )
    {
        return;
    }

    // Handle the case where wire is pullup or pulldown
    if ( pulled() )
    {
        switch ( pulled_state() )
        {
            case components::Wire::PulledStatus::PS_HIGH:
                state(State::PULLED_HIGH);
                break;

            case components::Wire::PulledStatus::PS_LOW:
                state(State::PULLED_LOW);
                break;

            default:
                throw common::StateError("Wire is pulled but pulled state not recognized!");
        }

        return;
    }

    switch ( state() )
    {
        case State::HIGH:           // Fallthrough
        case State::PULLED_HIGH:    // Fallthrough
        case State::FLOATING_HIGH:
            state(State::FLOATING_HIGH);
            break;

        case State::GROUNDED:       // Fallthrough
        case State::PULLED_LOW:     // Fallthrough
        case State::FLOATING_LOW:
            state(State::FLOATING_LOW);
            break;

        default:
            state(State::FLOATING);
            break;
    }
}


void Wire::set_high_low(const bool new_state) noexcept
{
    this->_state = new_state ? PULLED_HIGH : PULLED_LOW;
}


// Wire operators
Wire::operator std::string() const
{
    std::stringstream ss;

    ss << "Wire \"" << this->_primary_name << "\":\n";

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

    ss << "\tState:\t\t" << state_to_string(this->_state) << "\n";

    return ss.str();
}


bool Wire::operator==(const Wire &rhs) const
{
    bool equivalent = _id == rhs._id;

    equivalent &= _primary_name == rhs._primary_name;
    equivalent &= _other_names == rhs._other_names;

    equivalent &= _pulled == rhs._pulled;
    equivalent &= _special_type == rhs._special_type;
    equivalent &= _state == rhs._state;

    // Don't check function equality, it's not worth it

    equivalent &= _trans_gate_ids == rhs._trans_gate_ids;
    equivalent &= _trans_ctl_ids == rhs._trans_ctl_ids;

    return equivalent;
}


// Helper functions
const char *ERR_UNK_SPECIAL_WIRE = "Unknown special wire type specified: ";

std::string Wire::special_wire_name(const SpecialWireType type)
{
    switch( type )
    {
        case SW_VCC:       return WIRE_VCC;
        case SW_GND:       return WIRE_GND;
        default:
            throw std::runtime_error
            (
                ERR_UNK_SPECIAL_WIRE +
                std::to_string(type)
            );
    }
}


std::string Wire::state_to_string(const State state)
{
    switch( state )
    {
        case GROUNDED:          return ST_GND;
        case PULLED_LOW:        return ST_PLL;
        case FLOATING_LOW:      return ST_FLL;
        case FLOATING:          return ST_FLT;
        case FLOATING_HIGH:     return ST_FLH;
        case PULLED_HIGH:       return ST_PLH;
        case HIGH:              return ST_HGH;
        default:                return ST_UNK;
    }
}