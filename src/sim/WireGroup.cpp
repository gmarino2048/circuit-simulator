/**
 * @file WireGroup.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief This class contains the implementation for the WireGroup class
 *        which will provide groupings for wires 
 * @version 0.1
 * @date 2021-12-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <queue>
#include <string>
#include <vector>

#include <circsim/sim/WireGroup.hpp>

using WireGroup = circsim::sim::WireGroup;
using Wire = WireGroup::Wire;


/**
 * This method actually performs the aggregation of the wire group. It
 * works by collecting the control transistors for each wire, checking
 * whether they are on, and then processing any wires which have not already
 * been added to the wire group.
 */
void WireGroup::_build_wire_group(const size_t initial, const Database& database)
{
    _wires.insert(initial);
    std::queue<Wire*> unprocessed_wires
    ({
        database.get_wire(initial)
    });

    while( !unprocessed_wires.empty() )
    {
        Wire* next_wire = unprocessed_wires.front();
        unprocessed_wires.pop();

        const std::vector<size_t> ctrl_transistors = next_wire->ctrl_transistors();
        for( const size_t transistor_id : ctrl_transistors )
        {
            Transistor* transistor = database.get_transistor(transistor_id);

            // If the transistor is on, add the other wire to the unprocessed list
            if( transistor->current_state() == Transistor::ON )
            {
                size_t unprocessed_wire_id = 0;

                if( next_wire->id() == transistor->drain() )
                {
                    unprocessed_wire_id = transistor->source();
                }
                else if( next_wire->id() == transistor->source() )
                {
                    unprocessed_wire_id = transistor->drain();
                }
                else
                {
                    // The wire is not listed in the transistor src/drain
                    throw std::runtime_error
                    (
                        "Bad ctrl transistor assignment for wire " +
                        std::to_string(next_wire->id()) + " and transistor " +
                        std::to_string(transistor_id)
                    );
                }

                // Only add the wire id to the process list if it is NOT already in
                // the list of wires
                if( _wires.find(unprocessed_wire_id) == _wires.end() )
                {
                    _wires.insert(unprocessed_wire_id);

                    Wire* unproc_wire = database.get_wire(unprocessed_wire_id);
                    unprocessed_wires.push(unproc_wire);
                }
                // Otherwise there is a circular reference, but the wires will all
                // take on the same value anyway regardless

            }
            // Terminate if the transistor is off

        }
        // No more control transistors to process

    }
    // End when there are no more unprocessed wires
}


void WireGroup::_recalculate_group_state(const Database& database)
{
    // Check for GND wire
    if( _wires.find(Wire::GND_ID()) != _wires.end() )
    {
        _state = Wire::GROUNDED;
        return;
    }

    // Check for VCC wire
    if( _wires.find(Wire::VCC_ID()) != _wires.end() )
    {
        _state = Wire::HIGH;
        return;
    }

    uint8_t value = 0;
    size_t fl_count = 0;
    size_t fh_count = 0;
    for( const size_t wire_id : _wires )
    {
        Wire* wire = database.get_wire(wire_id);
        value |= static_cast<uint8_t>(wire->state());
    }

    // Note: The order of the following section is critical
    // Priority of states is determined by the ordering of "if" statements
#define SET_STATE(x) \
    if( value & (x) ) \
    { \
        _state = (x); \
        return; \
    }

    SET_STATE(Wire::GROUNDED);
    SET_STATE(Wire::HIGH);
    SET_STATE(Wire::PULLED_LOW);
    SET_STATE(Wire::PULLED_HIGH);

#undef SET_STATE

    // If we've made it this far, wire must be floating low or floating high
    // Check the floating counts to see which one is greater.
    // Note that if they are equal, we assume FLOATING_LOW since FLOATING will also
    // turn off a transistor
    _state = fl_count >= fh_count ? Wire::FLOATING_LOW : Wire::FLOATING_HIGH;
}


void WireGroup::_update_wire_states(const Database& database) const
{
    for( const size_t wire_id : _wires )
    {
        Wire* wire = database.get_wire(wire_id);

        if( (wire->id() == Wire::VCC_ID()) || (wire->id() == Wire::GND_ID()) )
        {
            continue;
        }

        switch( _state )
        {
            case Wire::GROUNDED:
                wire->state
                (
                    wire->pulled() ? Wire::PULLED_LOW : Wire::GROUNDED
                );
                break;

            case Wire::HIGH:
                wire->state
                (
                    wire->pulled() ? Wire::PULLED_HIGH : Wire::HIGH
                );
                break;

            default:
                wire->state(_state);
                break;
        }
    }
}


WireGroup::WireGroup() :
    _state(Wire::FLOATING)
{
    // All other members use default constructors
}


WireGroup::WireGroup(const size_t wire, const Database& database) : WireGroup()
{
    initialize(wire, database);
}


void WireGroup::reset()
{
    _state = Wire::FLOATING;
    _wires.clear();
}


void WireGroup::initialize(const size_t wire, const Database& database)
{
    reset();

    _build_wire_group(wire, database);
    _recalculate_group_state(database);
    _update_wire_states(database);
}
