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

#include <iomanip>
#include <string>
#include <sstream>

#include <circsim/components/Transistor.hpp>

using namespace circsim::components;

Transistor::Transistor(): _id(-1),
                          _gate_id(0),
                          _source_id(0),
                          _drain_id(0)
{
    // Default constructor for all other members
}


Transistor::Transistor
(
    const size_t id,
    const size_t gate_id,
    const size_t source_id,
    const size_t drain_id
):  _id(id),
    _gate_id(gate_id),
    _source_id(source_id),
    _drain_id(drain_id)
{
    // Default constructor for all other members
}


Transistor::Transistor
(
    const size_t id,
    const std::string &name,
    const size_t gate_id,
    const size_t source_id,
    const size_t drain_id
):  _id(id),
    _name(name),
    _gate_id(gate_id),
    _source_id(source_id),
    _drain_id(drain_id)
{
    // No other members to initialize
}


ssize_t Transistor::id() const noexcept
{
    return this->_id;
}


std::string Transistor::name() const noexcept
{
    return this->_name;
}


Transistor::operator std::string() const
{
    std::stringstream ss;

    ss << "Transistor \"" << this->_name << "\":\n";
    ss << "\tId:\t\t0x" << std::uppercase << std::setfill('0') << std::setw(16)
        << this->_id << "\n";
    ss << "\tGate:\t\t" << std::uppercase << std::setfill('0') << std::setw(16)
        << this->_gate_id << "\n";
    ss << "\tSource:\t\t" << std::uppercase << std::setfill('0') << std::setw(16)
        << this->_source_id << "\n";
    ss << "\tDrain:\t\t" << std::uppercase << std::setfill('0') << std::setw(16)
        << this->_drain_id << "\n";

    return ss.str();
}
