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


    /**
     * @brief Create the wire group given an initial wire and a database
     *        of components.
     * 
     * @param initial The wire used to build the group.
     * @param database The database of components used for reference.
     */
    void _build_wire_group(const size_t initial, const Database& database);

    /**
     * @brief Recalculate the wire group state given all of the wires in
     *        the group. Usually this should only be required once.
     * 
     * @param database The component database.
     */
    void _recalculate_group_state(const Database& database);

    /**
     * @brief Update the individual wire states in this wire group.
     * 
     * @param database The database used to get the wire instances.
     */
    void _update_wire_states(const Database& database) const;

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
    inline bool empty() const { return _wires.empty(); }


    /**
     * @brief Get the state of this wire group.
     * 
     * @return Wire::State The state of this wire group.
     */
    inline Wire::State group_state() const noexcept { return _state; };

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