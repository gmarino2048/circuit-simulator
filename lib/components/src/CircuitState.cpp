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
std::unordered_map<uint64_t, Transistor::State>& CircuitState::_get_state_map<Transistor>()
{
    return _transistor_states;
}

template<>
std::unordered_map<uint64_t, Wire::State>& CircuitState::_get_state_map<Wire>()
{
    return _wire_states;
}


CircuitState::CircuitState():
    _id(std::nullopt)
{}


CircuitState::CircuitState(const uint64_t id):
    _id(id)
{}


bool CircuitState::operator==(const CircuitState& rhs) const
{
    bool equivalent = true;

    equivalent &= _id == rhs._id;

    equivalent &= _transistor_states == rhs._transistor_states;
    equivalent &= _wire_states == rhs._wire_states;

    return equivalent;
}


template ObjectState<Transistor> CircuitState::get_state<Transistor>(const uint64_t id) const;
template ObjectState<Wire> CircuitState::get_state<Wire>(const uint64_t id) const;

template<class T>
ObjectState<T> CircuitState::get_state(const uint64_t id) const
{
    std::unordered_map<uint64_t, typename T::State>& map =
        const_cast<CircuitState*>(this)->_get_state_map<T>();

    using iterator_t = typename std::unordered_map<uint64_t, typename T::State>::const_iterator;
    if( iterator_t it = map.find(id); it != map.end() )
    {
        return ObjectState<T>(it->first, it->second);
    }
    else
    {
        throw circsim::common::ValueError
        (
            "Object with ID " + std::to_string(id) + " not found."
        );
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
    std::vector<ObjectState<T>> values;
    std::unordered_map<uint64_t, typename T::State> map =
        const_cast<CircuitState*>(this)->_get_state_map<T>();

    values.reserve(map.size());

    std::copy
    (
        map.begin(),
        map.end(),
        std::back_inserter(values)
    );

    return values;
}


template void CircuitState::update_object_state(const Transistor& object);
template void CircuitState::update_object_state(const Wire& object);

template<class T>
void CircuitState::update_object_state(const T& object)
{
    _get_state_map<T>()[object.id()] = _get_object_state<T>(object);
}


template void CircuitState::update_object_state_manual<Transistor>(const uint64_t id, const Transistor::State state);
template void CircuitState::update_object_state_manual<Wire>(const uint64_t id, const Wire::State state);

template<class T>
void CircuitState::update_object_state_manual(const uint64_t id, const typename T::State state)
{
    _get_state_map<T>()[id] = state;
}


template void CircuitState::update_multiple_states<Transistor>(const std::vector<Transistor>& objects);
template void CircuitState::update_multiple_states<Wire>(const std::vector<Wire>& objects);

template<class T>
void CircuitState::update_multiple_states(const std::vector<T>& objects)
{
    for( const T& object : objects )
    {
        update_object_state<T>(object);
    }
}


template void CircuitState::update_multiple_states_manual<Transistor>
(
    const std::vector<uint64_t>& ids,
    const std::vector<Transistor::State>& states
);
template void CircuitState::update_multiple_states_manual<Wire>
(
    const std::vector<uint64_t>& ids,
    const std::vector<Wire::State>& states
);

template<class T>
void CircuitState::update_multiple_states_manual
(
    const std::vector<uint64_t>& ids,
    const typename std::vector<typename T::State>& states
)
{
    if( ids.size() != states.size() )
    {
        throw circsim::common::ValueError
        (
            "ID list size does not match state list size"
        );
    }

    for( size_t i = 0; i < ids.size(); i++ )
    {
        update_object_state_manual<T>(ids[i], states[i]);
    }
}
