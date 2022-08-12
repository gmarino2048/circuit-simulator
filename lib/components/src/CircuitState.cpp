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
#include <unordered_map>
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
Transistor::State CircuitState::_get_object_state<Transistor>(const Transistor& object) const
{
    return object.current_state();
}

template<>
Wire::State CircuitState::_get_object_state<Wire>(const Wire& object) const
{
    return object.state();
}


template<>
std::vector<ObjectState<Transistor>>& CircuitState::_get_state_list<Transistor>()
{
    return _transistor_states;
}

template<>
std::vector<ObjectState<Wire>>& CircuitState::_get_state_list<Wire>()
{
    return _wire_states;
}


CircuitState::CircuitState():
    _id(std::nullopt)
{}


CircuitState::CircuitState(const uint64_t id):
    _id(id)
{}


template ObjectState<Transistor> CircuitState::get_state<Transistor>(const uint64_t id) const;
template ObjectState<Wire> CircuitState::get_state<Wire>(const uint64_t id) const;

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


template std::vector<ObjectState<Transistor>> CircuitState::get_states<Transistor>(const std::vector<uint64_t>& ids) const;
template std::vector<ObjectState<Wire>> CircuitState::get_states<Wire>(const std::vector<uint64_t>& ids) const;

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


template std::vector<ObjectState<Transistor>> CircuitState::get_all_states<Transistor>() const;
template std::vector<ObjectState<Wire>> CircuitState::get_all_states<Wire>() const;

template<class T>
std::vector<ObjectState<T>> CircuitState::get_all_states() const
{
    return const_cast<CircuitState*>(this)->_get_state_list<T>();
}


template void CircuitState::update_state<Transistor>(const Transistor& object);
template void CircuitState::update_state<Wire>(const Wire& object);

template<class T>
void CircuitState::update_state(const T& object)
{
    
}
