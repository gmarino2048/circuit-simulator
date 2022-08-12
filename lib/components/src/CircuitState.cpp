/**
 * @file CircuitState.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Implementation of the CircuitState object.
 * @version 0.1
 * @date 2022-08-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <algorithm>
#include <optional>
#include <vector>

// Library Includes
// (none)

// Project Includes
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>

#include <circsim/components/CircuitState.hpp>

using namespace circsim::components;

template<class T>
using ObjectState = CircuitState::ObjectState<T>;


template<>
std::vector<ObjectState<Transistor::State>>& CircuitState::_get_state_list()
{
    return _transistor_states;
}

template<>
std::vector<ObjectState<Wire::State>>& CircuitState::_get_state_list()
{
    return _wire_states;
}


CircuitState::CircuitState():
    _id(std::nullopt)
{}


CircuitState::CircuitState(const uint64_t id):
    _id(id)
{}


CircuitState::CircuitState
(
    const uint64_t id,
    const std::vector<Transistor>& transistor_list,
    const std::vector<Wire>& wire_list
):
    _id(id)
{
    _transistor_states.reserve(transistor_list.size());
    _wire_states.reserve(wire_list.size());

    std::transform
    (
        transistor_list.begin(),
        transistor_list.end(),
        std::back_inserter(_transistor_states),
        [](const Transistor& transistor)
        {
            return ObjectState<Transistor::State>(transistor.id(), transistor.current_state());
        }
    );

    std::transform
    (
        wire_list.begin(),
        wire_list.end(),
        std::back_inserter(_wire_states),
        [](const Wire& wire)
        {
            return ObjectState<Wire::State>(wire.id(), wire.state());
        }
    );
}


template ObjectState<Transistor::State> CircuitState::get_state(const uint64_t id) const;
template ObjectState<Wire::State> CircuitState::get_state(const uint64_t id) const;

template<class T>
ObjectState<T> CircuitState::get_state(const uint64_t id) const
{
    std::vector<ObjectState<T>>& state_list = const_cast<CircuitState*>(this)->_get_state_list<T>();
    typename std::vector<ObjectState<T>>::const_iterator it;

    it = std::find_if
    (
        state_list.begin(),
        state_list.end(),
        [id](const ObjectState<T> &state)
        {
            return state.first == id;
        }
    );

    if( it != state_list.end() )
    {
        return *it;
    }
    else
    {
        throw circsim::common::ValueError("No state with matching ID found.");
    }
}


template std::vector<ObjectState<Transistor::State>> CircuitState::get_states(const std::vector<uint64_t>& ids) const;
template std::vector<ObjectState<Wire::State>> CircuitState::get_states(const std::vector<uint64_t>& ids) const;

template<class T>
std::vector<ObjectState<T>> CircuitState::get_states(const std::vector<uint64_t>& ids) const
{
    std::vector<ObjectState<T>> states;
    for( const uint64_t id : ids )
    {
        states.push_back(get_state<T>(id));
    }
    return states;
}
