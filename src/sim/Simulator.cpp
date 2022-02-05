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

#include <list>
#include <pair>
#include <set>
#include <circsim/components/Transistor.hpp>
#include <circsim/sim/Simulator.hpp>

using namespace circsim;
using namespace circsim::sim;


void Simulator::_create_wire_group(const size_t wire_id, WireGroup &group)
{
    group = WireGroup(wire_id, this->_internal_database);
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
