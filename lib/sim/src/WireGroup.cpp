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

// C++ Stdlib Includes
#include <queue>
#include <string>
#include <stdexcept>
#include <vector>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/StateError.hpp>
#include <circsim/components/Circuit.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>
#include <circsim/sim/WireGroup.hpp>

using WireGroup = circsim::sim::WireGroup;
using Wire = WireGroup::Wire;


/**
 * This method actually performs the aggregation of the wire group. It
 * works by collecting the control transistors for each wire, checking
 * whether they are on, and then processing any wires which have not already
 * been added to the wire group.
 */
void WireGroup::_build_wire_group(const uint64_t initial, const Circuit& circuit)
{
    _wires.insert(initial);
    std::queue<Wire*> unprocessed_wires
    ({
        circuit.get<Wire>(initial)
    });

    while( !unprocessed_wires.empty() )
    {
        Wire* next_wire = unprocessed_wires.front();
        unprocessed_wires.pop();

        const std::vector<uint64_t> ctrl_transistors = next_wire->ctrl_transistors();
        for( const uint64_t transistor_id : ctrl_transistors )
        {
            Transistor* transistor = circuit.get<components::Transistor>(transistor_id);

            // If the transistor is on, add the other wire to the unprocessed list
            if( transistor->current_state() == Transistor::ON )
            {
                uint64_t unprocessed_wire_id = 0;

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

                    Wire* unproc_wire = circuit.get<Wire>(unprocessed_wire_id);
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


void WireGroup::_recalculate_group_state(const Circuit& circuit)
{
    uint8_t value = 0;
    size_t fl_count = 0;
    size_t fh_count = 0;
    for( const uint64_t wire_id : _wires )
    {
        Wire* wire = circuit.get<Wire>(wire_id);

        if( wire->special_type() == Wire::SpecialWireType::SW_GND )
        {
            value |= Wire::State::GROUNDED;
            break;
        }
        else if( wire->special_type() == Wire::SpecialWireType::SW_VCC )
        {
            value |= Wire::State::HIGH;
        }

        wire->set_floating();

        if ( wire->state() == Wire::State::FLOATING_HIGH )
        {
            fh_count++;
        }
        else if ( wire->state() == Wire::State::FLOATING_LOW )
        {
            fl_count++;
        }
        
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


void WireGroup::_update_wire_states(const Circuit& circuit) const
{
    for( const uint64_t wire_id : _wires )
    {
        Wire* wire = circuit.get<Wire>(wire_id);

        bool is_vcc = wire->special_type() == Wire::SW_VCC;
        bool is_gnd = wire->special_type() == Wire::SW_GND;

        if( is_vcc || is_gnd )
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


WireGroup::WireGroup(const uint64_t wire, const Circuit& circuit) : WireGroup()
{
    initialize(wire, circuit);
}


void WireGroup::reset()
{
    _state = Wire::FLOATING;
    _wires.clear();
}


void WireGroup::initialize(const uint64_t wire, const Circuit& circuit)
{
    reset();

    _build_wire_group(wire, circuit);
    _recalculate_group_state(circuit);
    _update_wire_states(circuit);
}


std::vector<uint64_t> WireGroup::gate_transistors(const Circuit &circuit) const
{
    std::vector<uint64_t> transistor_ids;

    for ( const uint64_t wire_id : _wires )
    {
        const Wire *wire_object = circuit.get<Wire>(wire_id);

        std::vector<uint64_t> gate_ids = wire_object->gate_transistors();
        for( const uint64_t trans_id : gate_ids )
        {
            transistor_ids.push_back(trans_id);
        }
    }

    return transistor_ids;
}
