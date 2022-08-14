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
#include <circsim/data/InternalStorage.hpp>

namespace circsim::sim
{

/**
 * @brief This object represents a group of connected wires within the
 *        simulator. Since the wires are all connected, they will all share
 *        the same state, which is determined by analyzing the wire state of
 *        all wires in the group.
 * 
 */
class WireGroup final
{
public:

    /// Import easy-access definition for Wire
    using Wire = ::circsim::components::Wire;

    /// Import easy-access definition for Transistor
    using Transistor = ::circsim::components::Transistor;

    /// Import easy-access definition for Circuit
    using Circuit = ::circsim::components::Circuit;

private:

    /// The current state of the wire group. This will be UNKNOWN
    /// if the group state has not been calculated.
    Wire::State _state;

    /// The set of all wires in this group. Wires must not be duplicated.
    std::set<uint64_t> _wires;


    /**
     * @brief Create the wire group given an initial wire and a circuit
     *        definition.
     * 
     * @param initial The wire used to build the group.
     * @param circuit The circuit used for reference.
     */
    void _build_wire_group(const uint64_t initial, const Circuit& circuit);

    /**
     * @brief Recalculate the wire group state given all of the wires in
     *        the group. Usually this should only be required once.
     * 
     * @param circuit The circuit object
     */
    void _recalculate_group_state(const Circuit& circuit);

    /**
     * @brief Update the individual wire states in this wire group.
     * 
     * @param circuit The circuit used to get the wire instances.
     */
    void _update_wire_states(const Circuit& circuit) const;

public:

    /**
     * @brief Default Constructor.
     * 
     */
    WireGroup();

    /**
     * @brief Constructs a new object given a wire and the circuit definition.
     *        This will automatically find all wires connected to the given one.
     * 
     * @param wire The initial wire used to construct the group.
     * @param circuit The circuit from which the components are gathered.
     */
    WireGroup(const uint64_t wire, const Circuit& circuit);


    /**
     * @brief Reset the group to its initial empty state.
     * 
     */
    void reset();

    /**
     * @brief Initialize the group using a wire and a circuit. This method will
     *        comb the circuit and assemble a group of connected wires given the
     *        gate states of each transistor.
     * 
     * @param wire The initial wire used to create the group.
     * @param circuit The database used for component information.
     */
    void initialize(const uint64_t wire, const Circuit& circuit);


    /**
     * @brief Determines whether this wire group is empty
     * 
     * @return true The wire group is empty.
     * @return false The wire group is not empty.
     */
    inline bool empty() const { return _wires.empty(); }


    /**
     * @brief Get the set of wire IDs in the wire group, so that they
     *        can be iterated over if needed.
     * 
     * @return std::set<uint64_t> The IDs of the wire in the set.
     */
    inline std::set<uint64_t> wire_ids() const { return _wires; }


    /**
     * @brief Get the state of this wire group.
     * 
     * @return Wire::State The state of this wire group.
     */
    inline Wire::State group_state() const noexcept { return _state; };

    /**
     * @brief Get the gate transistors connected to this wire group.
     * 
     * @details This function will return a group of unique transistor IDs whose
     *          gate nodes are connected to this wire group. This will allow us to
     *          easily determine which transistors are going to switch.
     * 
     * @param circuit The component circuit to use.
     * 
     * @return std::vector<uint64_t> The IDs of the gate transistors for the group.
     */
    std::vector<uint64_t> gate_transistors(const Circuit &circuit) const;

};

}

#endif