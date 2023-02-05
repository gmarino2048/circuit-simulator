/**
 * @file Clock.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Clock module responsible for tracking ticks and changing wire
 *        status between ticks.
 * @version 0.1
 * @date 2023-02-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// C++ Stdlib Includes
#include <cinttypes>
#include <vector>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Clock.hpp>
#include <circsim/components/Wire.hpp>

using namespace circsim::components;

void Clock::_update_wires(const Wire::State new_state)
{
    // Do nothing if the circuit is registered as NULL
    if(_circuit == nullptr)
    {
        return;
    }

    // For each wire in the update list, get the wire and
    // set its state
    std::vector<Wire*> wire_objects;
    for(const uint64_t wire_id : _wires)
    {
        Wire* wire = _circuit->get<Wire>(wire_id);
        wire_objects.push_back(wire);
    }

    // Wait to make sure we can get all wires before updating them
    for(Wire* wire : wire_objects)
    {
        wire->state(new_state);
    }
}

Clock::Clock(std::vector<uint64_t> wires) :
    _halftick_counter(0U),
    _current_state(Wire::State::PULLED_LOW),
    _wires(wires),
    _circuit(nullptr)
{}


void Clock::update_wires(const std::vector<uint64_t> new_list)
{
    _wires = new_list;
    reset_counter();
}


uint64_t Clock::halftick_count() const
{
    return _halftick_counter;
}


uint64_t Clock::tick_count() const
{
    return (_halftick_counter / 2U);
}


void Clock::reset_counter()
{
    _halftick_counter = 0U;
    _update_wires(Wire::State::PULLED_LOW);
}