/**
 * @file CircuitState.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief This class contains a snapshot of the state of the circuit at some given
 *        time. This information can be used to analyze the circuit operation at a
 *        later date.
 * @version 0.1
 * @date 2022-08-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_COMPONENTS_CIRCUITSTATE_HPP
#define __CIRCSIM_COMPONENTS_CIRCUITSTATE_HPP

// C++ Stdlib Includes
#include <optional>
#include <utility>
#include <vector>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>
#include <circsim/data/InternalStorage.hpp>


namespace circsim::components
{

class CircuitState final
{
public:

    typedef std::pair<uint64_t, Transistor::State> TransistorState;

    typedef std::pair<uint64_t, Wire::State> WireState;

private:

    std::optional<uint64_t> _id;

    std::vector<TransistorState> _transistor_states;

    std::vector<WireState> _wire_state;

public:

    CircuitState();

    CircuitState(const uint64_t id);

    CircuitState
    (
        const uint64_t id,
        const std::vector<Transistor>& transistor_list,
        const std::vector<Wire>& wire_list
    );


    inline uint64_t id() const try
    {
        return _id.value();
    }
    catch(const std::bad_optional_access&)
    {
        throw circsim::common::ValueError("Transistor currently has no ID");
    }


    template<class T>
    T::State get_state(const uint64_t id) const;

    template<class T>
    std::vector<T::State> get_states(const std::vector<uint64_t>& ids) const;

    template<class T>
    std::vector<T::State> get_all_states() const;


    template<class T>
    void update_state(const T& object);

    template<class T>
    void update_all_states(const std::vector<T>& objects);

};  // end class CircuitState

}   // end namespace circsim::components

#endif // #ifndef __CIRCSIM_COMPONENTS_CIRCUITSTATE_HPP
