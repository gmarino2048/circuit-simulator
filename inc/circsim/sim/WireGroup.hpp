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

    /// Import easy-access definition for Wire
    using Wire = ::circsim::components::Wire;

    /// Import easy-access definition for Transistor
    using Transistor = ::circsim::components::Transistor;

    /// Import component database type
    using Database = ::circsim::data::InternalDatabase;

private:

    /// The current state of the wire group. This will be UNKNOWN
    /// if the group state has not been calculated.
    Wire::State _state;

    /// The set of all wires in this group. Wires must not be duplicated.
    std::set<size_t> _wires;

public:

    /**
     * @brief Default Constructor.
     * 
     */
    WireGroup();

    /**
     * @brief Constructs a new object given a wire and the component database.
     *        This will automatically find all wires connected to the given one.
     * 
     * @param wire The initial wire used to construct the group.
     * @param database The database from which the components are gathered.
     */
    WireGroup(const size_t wire, const Database& database);


    /**
     * @brief Reset the group to its initial empty state.
     * 
     */
    void reset();

    /**
     * @brief Initialize the group using a wire and a database. This method will
     *        comb the database and assemble a group of connected wires given the
     *        gate states of each transistor.
     * 
     * @param wire The initial wire used to create the group.
     * @param database The database used for component information.
     */
    void initialize(const size_t wire, const Database& database);


    /**
     * @brief Determines whether this wire group is empty
     * 
     * @return true The wire group is empty.
     * @return false The wire group is not empty.
     */
    bool empty() const;


    /**
     * @brief Get the state of this wire group.
     * 
     * Note that the group state is cached in the member variable
     * _state, and the recalculate parameter allows us to force recalculation
     * if necessary.
     * 
     * @param recalculate Whether to recalculate the wire state.
     * @return Wire::State The state of this wire group.
     */
    Wire::State group_state(const bool recalculate = false);

    /**
     * @brief Get the gate transistors connected to this wire group.
     * 
     * This function will return a group of unique transistor IDs whose
     * gate nodes are connected to this wire group. This will allow us to
     * easily determine which transistors are going to switch.
     * 
     * @return std::vector<size_t> The IDs of the gate transistors for the group.
     */
    std::vector<size_t> gate_transistors() const;

};

}

#endif