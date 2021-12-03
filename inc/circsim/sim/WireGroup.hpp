/**
 * @file WireGroup.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief This class contains the definition for the WireGroup
 *        class, which will allow the simulator to easily construct
 *        a group of connected wires given the component database.
 *        The WireGroup class will pull the connected wires from the
 *        database and allow easy state gathering.
 * @version 0.1
 * @date 2021-12-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CIRCSIM_SIM_WIREGROUP_HPP
#define __CIRCSIM_SIM_WIREGROUP_HPP

#include <cstdint>
#include <set>
#include <vector>

#include <circsim/components/Wire.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/data/InternalDatabase.hpp>

namespace circsim::sim
{

class WireGroup final
{
public:

    using Wire = ::circsim::components::Wire;
    using Transistor = ::circsim::components::Transistor;

    using Database = ::circsim::data::InternalDatabase;

private:

    Wire::State _state;
    std::set<size_t> _wires;

public:

    WireGroup();
    WireGroup(const size_t wire, const Database& database);

    void reset();
    void initialize(const size_t wire, const Database& database);

    Wire::State group_state();
    std::vector<size_t> gate_transistors() const;

};

}

#endif