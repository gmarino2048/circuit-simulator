/**
 * @file Simulator.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief This file contains the definition for the main
 *        simulator object. This object will be responsible for
 *        all of the updates within the circuit itself.
 * @version 0.1
 * @date 2022-01-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <cstdint>
#include <list>
#include <string>
#include <vector>

#include <circsim/components/Circuit.hpp>
#include <circsim/components/Wire.hpp>
#include <circsim/sim/WireGroup.hpp>

namespace circsim::sim
{

/**
 * @brief This is the simulator class responsible for making the updates to the
 *        transistors and wires within the circuit.
 * 
 * The simulator follows a series of basic steps to make updates to the circuit.
 * It maintains a list of wires which need to be updated, and then creates a wire
 * group from each of those wires. Once that is complete, it updates the transistors
 * whose gate side is connected to that wire group. For any of the transistors which
 * change state, the other wire is then added to the list of wires which need
 * recalculation.
 * 
 */
class Simulator final
{
public:

    /// Easy-access definition for the wire state enum
    using WireState = components::Wire::State;

private:

    /// The current iteration count of the simulator
    size_t _iteration_count;

    /// The iteration limit of the simulator
    size_t _iteration_limit;

    /// The list of wires which need to be updated
    std::list<uint64_t> _wire_update_list;

    /// The internal component circuit for the simulator
    components::Circuit _circuit;


    /**
     * @brief Create a wire group from a given wire ID. The Wire group is
     *        then filled into the reference provided. This allows a single
     *        wire group to be reused rather than reallocating memory every
     *        iteration.
     * 
     * @param wire_id The ID of the initial wire used to create the group.
     * @param group The receiving reference for the wire group.
     */
    void _create_wire_group(const uint64_t wire_id, WireGroup &group);


    /**
     * @brief Update all the gate-side transistors from the wire group. If
     *        the transistor changes state then add the proper wires to the
     *        update list.
     * 
     * @param group The WireGroup used to update the transistors
     */
    void _update_transistors(const WireGroup &group);

public:

    /**
     * @brief Default constructor for the Simulator
     * 
     * @param iteration_limit The iteration limit for the simulator (default 2,500)
     */
    Simulator(const size_t iteration_limit = 2500);

    /**
     * @brief Construct a new Simulator object with an existing circuit
     * 
     * @param circuit The circuit to use for the simulator
     * @param iteration_limit The limit of iterations to resolve changes in the simulator (default 2500)
     */
    Simulator
    (
        const components::Circuit &circuit,
        const size_t iteration_limit = 2500
    );


    /**
     * @brief Get the internal circuit of the simulator
     * 
     * @return components::Circuit& The internal circuit object
     */
    inline components::Circuit& circuit() { return _circuit; }

    /**
     * @brief Update the internal component circuit with a newer version.
     * 
     * Once the circuit has been updated, we need to reset the update list and
     * the counter, as they will no longer be valid.
     * 
     * @param circuit The new circuit to use.
     */
    inline void circuit(const components::Circuit &circuit) 
    {
        this->_circuit = circuit;
    }


    /**
     * @brief Get the iteration limit for the simulator.
     * 
     * @return size_t The iteration limit
     */
    inline size_t iteration_limit() { return _iteration_limit; }

    /**
     * @brief Set the iteration limit for the simulator
     * 
     * @param count The iteration limit
     */
    inline void iteration_limit(const size_t count) { _iteration_limit = count; }


    /**
     * @brief Run a single iteration of the update algorithm. This will
     *        only work on the next wire that needs to be updated.
     * 
     * @details This method will **not** increase the iteration counter,
     *          as it only performs a single run, and the iteration limit
     *          is designed to prevent runaway automatic operations.
     * 
     * @return true if there are no more wires to update
     * @return false if there are more wires to update
     */
    bool update_next();

    /**
     * @brief Update all the remaining wires in the queue until convergence is
     *        reached or until the iteration limit is reached.
     * 
     * @details This method first resets the iteration count, then runs the
     *          update algorithm until convergence is reached or the iteration
     *          limit is reached.
     * 
     * @throw circsim::common::LimitError if the limit is exceeded
     */
    void update_all();


    /**
     * @brief Reset the circuit to its original, powered off state.
     * 
     */
    void reset_circuit();

    /**
     * @brief Removes all elements from the update list, leaving the current
     *        state of the processor untouched.
     * 
     */
    void reset_update_list();

    /**
     * @brief Reset the current iteration counter to 0, allowing more
     *        iterations to be performed on a subsequent pass.
     * 
     */
    inline void reset_iteration_count()
    {
        this->_iteration_count = 0;
    }


    /**
     * @brief Mark a wire as changed after an update.
     * 
     * @param id The wire ID to mark as updated
     * @param update_all If set, this call will update the circuit
     *                   state until convergence or the limit is reached.
     */
    void mark_updated
    (
        const uint64_t id,
        const bool update_all = true
    );

    /**
     * @brief Mark a series of wires as changed after an update.
     * 
     * @param ids The wire ID list to mark as updated
     * @param update_all If set, this call will update the circuit
     *                   state until convergence or the limit is reached.
     */
    void mark_all_updated
    (
        const std::vector<uint64_t>& ids,
        const bool update_all = true
    );


    /**
     * @brief Update a wire using its ID and a given State. This method
     *        will change the wire to the provided state and add the wire
     *        to the simulator's internal recalculation list.
     * 
     * @param id The wire ID to modify
     * @param state The new state of the wire
     * @param update_all If set, this call will continue until the state
     *                   of the chip converges or the limit is reached.
     * 
     * @throw circsim::common::IndexError if the wire ID cannot be found
     * @throw circsim::common::LimitError if the update limit is exceeded
     */
    void update_by_id
    (
        const uint64_t id,
        const WireState state,
        const bool update_all = true
    );

    /**
     * @brief Update a wire by searching for the name and then changing the
     *        value to the one provided, then will add the wire to the
     *        simulator's recalculation list.
     * 
     * @param name The wire name to search for
     * @param state The new wire state to set
     * @param update_all If set, this call will continue until the state
     *                   of the chip converges or the limit is reached.
     * 
     * @throw circsim::common::IndexError if the wire name cannot be found
     * @throw circsim::common::LimitError if the update limit is exceeded
     */
    void update_by_name
    (
        const std::string &name,
        const WireState state,
        const bool update_all = true
    );

    /**
     * @brief Update a register in the simulator and automatically mark
     *        all associated wires as modified.
     * 
     * @tparam T The type of integer value to use to set the register
     * @param register_id The ID of the register to update
     * @param value The integer value to set the register with
     * @param update_all Whether to trigger the update funciton after
     *                   the register has been updated.
     */
    template<class T>
    void update_by_register
    (
        const uint64_t register_id,
        const T value,
        const bool update_all = true
    );

    /**
     * @brief Update the list of provided wire IDs to the desired state.
     *        The value of the ID corresponds to the matching state in
     *        the state list. The lists must be the same size for this to
     *        work.
     * 
     * @param id_list The list of wire IDs
     * @param state_list The list of wire states to update to
     * @param update_all If set, this call will continue until the state
     *                   of the chip converges or the limit is reached.
     * 
     * @throw circsim::common::IndexError if the wire ID cannot be found
     * @throw circsim::common::LimitError if the update limit is exceeded
     * @throw circsim::common::FormatError if the size of the id list does not
     *        match that of the state list
     */
    void update_all_by_id
    (
        const std::vector<uint64_t> &id_list,
        const std::vector<WireState> &state_list,
        const bool update_all = true
    );

    /**
     * @brief Update the list of provided wires to the list of provided
     *        states. The two lists follow a one-to-one correlation when
     *        updating, searching for the wire which matches the desired
     *        name and then setting it to the provided state.
     * 
     * @param name_list The list of wire names
     * @param state_list The list of wire states
     * @param update_all If set, this call will continue until the state
     *                   of the chip converges or the limit is reached.
     * 
     * @throw circsim::common::IndexError if the wire name cannot be found
     * @throw circsim::common::LimitError if the update limit is exceeded
     * @throw circsim::common::FormatError if the size of the id list does not
     *        match that of the state list
     */
    void update_all_by_name
    (
        const std::vector<std::string> &name_list,
        const std::vector<WireState> &state_list,
        const bool update_all = true
    );

};

}