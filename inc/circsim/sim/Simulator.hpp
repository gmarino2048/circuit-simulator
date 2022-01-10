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
#include <queue>
#include <string>
#include <vector>

#include <circsim/components/Wire.hpp>
#include <circsim/data/InternalDatabase.hpp>
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
    std::queue<size_t> _wire_update_list;

    /// The internal component database for the simulator
    data::InternalDatabase _internal_database;

    void _create_wire_group(const size_t wire_id, WireGroup &group);
    void _update_transistors(const WireGroup &group);

public:

    Simulator(const size_t iteration_limit = 2500);

    Simulator
    (
        const data::InternalDatabase &database,
        const size_t iteration_limit = 2500
    );

    inline data::InternalDatabase& database() { return _internal_database; }
    inline void database(const data::InternalDatabase &database) { _internal_database = database; }

    inline size_t iteration_limit() { return _iteration_limit; }
    inline void iteration_limit(const size_t count) { _iteration_limit = count; }

    void update_next();
    void update_all();

    void reset_iteration_count();

    void update_by_id
    (
        const size_t id,
        const WireState state,
        const bool update = true
    );

    void update_by_name
    (
        const std::string &name,
        const WireState state,
        const bool update = true
    );

    void update_all_by_id
    (
        const std::vector<size_t> &id_list,
        const std::vector<WireState> &state_list,
        const bool update = true
    );

    void update_all_by_name
    (
        const std::vector<std::string> &name_list,
        const std::vector<WireState> &state_list,
        const bool update = true
    );

};

}