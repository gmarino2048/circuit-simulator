/**
 * @file Simulator.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief The definition file for the simulator object.
 * @version 0.1
 * @date 2022-01-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <algorithm>
#include <list>
#include <set>
#include <type_traits>
#include <utility>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/FormatError.hpp>
#include <circsim/common/LimitError.hpp>
#include <circsim/common/StateError.hpp>
#include <circsim/components/Circuit.hpp>
#include <circsim/components/Register.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>
#include <circsim/sim/WireGroup.hpp>
#include <circsim/sim/Simulator.hpp>

using namespace circsim;
using namespace circsim::sim;


void Simulator::_create_wire_group(const uint64_t wire_id, WireGroup &group)
{
    group = WireGroup(wire_id, this->_circuit);

    // Remove the wires in the group from the simulator's list for efficiency
    std::set<uint64_t> group_wire_ids = group.wire_ids();
    for
    (
        std::list<uint64_t>::const_iterator it = _wire_update_list.begin();
        it != _wire_update_list.end();
        it++
    )
    {
        uint64_t wire_id = *it;
        std::set<uint64_t>::const_iterator group_iterator = group_wire_ids.find(wire_id);

        if( group_iterator != group_wire_ids.end() )
        {
            it = _wire_update_list.erase(it);
        }
    }
}


void Simulator::_update_transistors(const WireGroup &group)
{
    // Convert transistor Ids to objects
    std::vector<uint64_t> transistors_to_update =
        group.gate_transistors(_circuit);

    for( const uint64_t id : transistors_to_update )
    {
        components::Transistor *transistor_object =
            _circuit.get<components::Transistor>(id);

        bool state_changed = transistor_object->update_state(group.group_state());
        bool transistor_initialized = transistor_object->is_initialized();

        // If the gate state has not changed, then move on to the next transistor
        if( (!state_changed) && transistor_initialized )
        {
            continue;
        }
        else if( !transistor_initialized )
        {
            transistor_object->set_initialized();
        }

        // Don't add if they're already in the update list
        std::list<uint64_t>::const_iterator source_update, drain_update;

        source_update = std::find
        (
            _wire_update_list.begin(),
            _wire_update_list.end(),
            transistor_object->source()
        );

        drain_update = std::find
        (
            _wire_update_list.begin(),
            _wire_update_list.end(),
            transistor_object->drain()
        );

        // Different actions depending on the state of the transistor
        if( transistor_object->current_state() == components::Transistor::ON )
        {
            // Wires are now connected
            // If neither is in the update list, add the source
            bool add_source = 
                ( source_update == _wire_update_list.end() ) &&
                ( drain_update == _wire_update_list.end() );

            if( add_source )
            {
                _wire_update_list.push_back(transistor_object->source());
            }
        }
        else
        {
            // Wires are now disconnected
            // No need to set floating since WireGroup will do this for us

            // Only add source/drain if they're not already in update list
            if( source_update == _wire_update_list.end() )
            {
                _wire_update_list.push_back(transistor_object->source());
            }
            if( drain_update == _wire_update_list.end() )
            {
                _wire_update_list.push_back(transistor_object->drain());
            }
        }
    }
}


Simulator::Simulator(const size_t iteration_limit) :
    _iteration_count(0),
    _iteration_limit(iteration_limit),
    _wire_update_list(),
    _circuit()
{ }


Simulator::Simulator
(
    const components::Circuit &circuit,
    const size_t iteration_limit
) :
    _iteration_count(0),
    _iteration_limit(iteration_limit),
    _wire_update_list()
{
    this->circuit(circuit);
}


bool Simulator::update_next()
{
    // Get the next wire to run the simulation on
    if ( _wire_update_list.size() == 0 )
    {
        return true;
    }

    uint64_t next_wire = _wire_update_list.front();
    _wire_update_list.pop_front();

    WireGroup wire_group;
    _create_wire_group(next_wire, wire_group);
    _update_transistors(wire_group);

    return false;
}


void Simulator::update_all()
{
    reset_iteration_count();

    while( _iteration_count < _iteration_limit )
    {
        bool finished = update_next();

        if ( finished )
        {
            return;
        }
    }

    throw common::LimitError("Iteration Limit Exceeded");
}


void Simulator::mark_updated
(
    const uint64_t id,
    const bool update_all
)
{
    _wire_update_list.push_back(id);

    if( update_all )
    {
        this->update_all();
    }
}


void Simulator::mark_all_updated
(
    const std::vector<uint64_t>& ids,
    const bool update_all
)
{
    std::copy
    (
        ids.begin(),
        ids.end(),
        std::back_inserter(_wire_update_list)
    );

    if( update_all )
    {
        this->update_all();
    }
}


void Simulator::update_by_id
(
    const uint64_t id,
    const WireState state,
    const bool update_all
)
{
    components::Wire *wire_object =
        _circuit.get<components::Wire>(id);

    // Preserve the internal order of the update list
    wire_object->state(state);
    _wire_update_list.push_back(wire_object->id());

    if( update_all )
    {
        this->update_all();
    }
}


void Simulator::update_by_name
(
    const std::string &name,
    const WireState state,
    const bool update_all
)
{
    components::Wire *wire_object =
        _circuit.find<components::Wire>(name);

    // Preserve the internal order of the update list
    wire_object->state(state);
    _wire_update_list.push_back(wire_object->id());

    if( update_all )
    {
        this->update_all();
    }
}


template void Simulator::update_by_register(const uint64_t, const uint8_t, const bool);
template void Simulator::update_by_register(const uint64_t, const uint16_t, const bool);
template void Simulator::update_by_register(const uint64_t, const uint32_t, const bool);
template void Simulator::update_by_register(const uint64_t, const uint64_t, const bool);

template void Simulator::update_by_register(const uint64_t, const int8_t, const bool);
template void Simulator::update_by_register(const uint64_t, const int16_t, const bool);
template void Simulator::update_by_register(const uint64_t, const int32_t, const bool);
template void Simulator::update_by_register(const uint64_t, const int64_t, const bool);

template<class T>
void Simulator::update_by_register
(
    const uint64_t register_id,
    const T value,
    const bool update_all
)
{
    using Register = components::Register;

    Register* reg = _circuit.get<Register>(register_id);

    if constexpr ( std::is_signed<T>::value )
    {
        reg->value_signed(value);
    }
    else if ( std::is_unsigned<T>::value )
    {
        reg->value_unsigned(value);
    }

    mark_all_updated
    (
        reg->wire_ids(),
        update_all
    );
}


void Simulator::update_all_by_id
(
    const std::vector<uint64_t> &id_list,
    const std::vector<WireState> &state_list,
    const bool update_all
)
{
    // First check the size of the two lists
    if ( id_list.size() != state_list.size() )
    {
        throw circsim::common::FormatError
        (
            "The size of the wire ID list ("
            + std::to_string(id_list.size())
            + ") must match that of the wire state list ("
            + std::to_string(state_list.size())
            + ")"
        );
    }

    for ( size_t i = 0; i < id_list.size(); i++ )
    {
        uint64_t wire_id = id_list[i];
        WireState wire_state = state_list[i];

        update_by_id(wire_id, wire_state, false);
    }

    if ( update_all )
    {
        this->update_all();
    }
}


void Simulator::update_all_by_name
(
    const std::vector<std::string> &name_list,
    const std::vector<WireState> &state_list,
    const bool update_all
)
{
    // First check the size of the two lists
    if ( name_list.size() != state_list.size() )
    {
        throw circsim::common::FormatError
        (
            "The size of the wire name list ("
            + std::to_string(name_list.size())
            + ") must match that of the wire state list ("
            + std::to_string(state_list.size())
            + ")"
        );
    }

    for ( size_t i = 0; i < name_list.size(); i++ )
    {
        std::string wire_name = name_list[i];
        WireState wire_state = state_list[i];

        update_by_name(wire_name, wire_state, false);
    }

    if ( update_all )
    {
        this->update_all();
    }
}
