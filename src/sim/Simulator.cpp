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

#include <algorithm>
#include <list>
#include <set>
#include <utility>

#include <circsim/common/FormatError.hpp>
#include <circsim/common/LimitError.hpp>
#include <circsim/common/StateError.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>
#include <circsim/sim/Simulator.hpp>

using namespace circsim;
using namespace circsim::sim;


void Simulator::_create_wire_group(const size_t wire_id, WireGroup &group)
{
    group = WireGroup(wire_id, this->_internal_database);

    // Remove the wires in the group from the simulator's list for efficiency
    std::set<size_t> group_wire_ids = group.wire_ids();
    for
    (
        std::list<size_t>::const_iterator it = _wire_update_list.begin();
        it != _wire_update_list.end();
        it++
    )
    {
        size_t wire_id = *it;
        std::set<size_t>::const_iterator group_iterator = group_wire_ids.find(wire_id);

        if( group_iterator != group_wire_ids.end() )
        {
            it = _wire_update_list.erase(it);
        }
    }
}


void Simulator::_update_transistors(const WireGroup &group)
{
    // Forward Declare Type
    using ControlWirePair = std::pair<size_t, size_t>;

    // Convert transistor Ids to objects
    std::vector<size_t> transistors_to_update =
        group.gate_transistors(_internal_database);

    for( const size_t id : transistors_to_update )
    {
        components::Transistor *transistor_object =
            _internal_database.get_transistor(id);

        bool state_changed = transistor_object->update_state(group.group_state());

        // If the gate state has not changed, then move on to the next transistor
        if( !state_changed )
        {
            continue;
        }

        // Don't add if they're already in the update list
        std::list<size_t>::const_iterator source_update, drain_update;

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
    _internal_database()
{ }


Simulator::Simulator
(
    const data::InternalDatabase &database,
    const size_t iteration_limit
) :
    _iteration_count(0),
    _iteration_limit(iteration_limit),
    _wire_update_list()
{
    this->database(database);
}


bool Simulator::update_next()
{
    // Get the next wire to run the simulation on
    if ( _wire_update_list.size() == 0 )
    {
        return true;
    }

    size_t next_wire = _wire_update_list.front();
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


void Simulator::update_by_id
(
    const size_t id,
    const WireState state,
    const bool update_all
)
{
    components::Wire *wire_object =
        _internal_database.get_wire(id);

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
        _internal_database.find_wire(name);

    // Preserve the internal order of the update list
    wire_object->state(state);
    _wire_update_list.push_back(wire_object->id());

    if( update_all )
    {
        this->update_all();
    }
}


void Simulator::update_all_by_id
(
    const std::vector<size_t> &id_list,
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
        size_t wire_id = id_list[i];
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
