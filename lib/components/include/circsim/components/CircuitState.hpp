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
#include <cinttypes>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>


namespace circsim::components
{

/**
 * @brief This object contains a snapshot of the state of the circuit
 *        at the time it was created. However, the list of states is
 *        editable.
 * 
 */
class CircuitState final
{
public:

    /**
     * @brief Type alias for the state of an individual object.
     * 
     * @tparam T The object type
     */
    template<class T>
    using ObjectState = std::pair<uint64_t, typename T::State>;

private:

    /// The ID of this object. This is not to be confused with its timestamp.
    std::optional<uint64_t> _id;

    /// A list of the states of all transistors registered in this object.
    std::unordered_map<uint64_t, Transistor::State> _transistor_states;

    /// A list of the states of all wires registered in this object.
    std::unordered_map<uint64_t, Wire::State> _wire_states;


    /**
     * @brief Get the state of an individual object.
     * 
     * @tparam T The type of the object
     * @param object The object instance
     * @return T::State The object's state value
     */
    template<class T>
    typename T::State _get_object_state(const T& object) const;


    /**
     * @brief Gets the state map for the associated object.
     * 
     * @tparam T The type of the object
     * @return std::vector<ObjectState<T>>& A reference to the state map
     */
    template<class T>
    std::unordered_map<uint64_t, typename T::State>& _get_state_map();

public:

    /**
     * @brief Default constructor.
     * 
     */
    CircuitState();

    /**
     * @brief Creates an empty CircuitState object.
     * 
     * @param id The ID of the newly created object
     */
    CircuitState(const uint64_t id);


    /**
     * @brief Gets the ID of this object with error checking.
     * 
     * @return uint64_t The object ID
     */
    inline uint64_t id() const try
    {
        return _id.value();
    }
    catch(const std::bad_optional_access&)
    {
        throw circsim::common::ValueError("Transistor currently has no ID");
    }


    /**
     * @brief Get the state of an individual object.
     * 
     * @tparam T The type of object to get state for
     * @param id The ID of the object
     * @return T::State The object's state
     */
    template<class T>
    ObjectState<T> get_state(const uint64_t id) const;

    /**
     * @brief Get a list of object states based on a list of object IDs.
     * 
     * @tparam T The type of object to get states for
     * @param ids The IDs to fetch state for
     * @return std::vector<T::State> The object states for the provided IDs
     */
    template<class T>
    std::vector<ObjectState<T>> get_states(const std::vector<uint64_t>& ids) const;

    /**
     * @brief Get all the states of the given object type contained in this object.
     * 
     *        Note: The objects are not guaranteed to be in any form of order.
     * 
     * @tparam T The type of object to get state for
     * @return std::vector<T::State> A list of all states, in order by ID
     */
    template<class T>
    std::vector<ObjectState<T>> get_all_states() const;


    /**
     * @brief Update the state of an existing object, or add it if
     *        it does not already exist.
     * 
     * @tparam T The type of the object
     * @param object The object to update
     */
    template<class T>
    void update_state(const T& object);

    /**
     * @brief Update the state of an existing object using only the ID and
     *        the associated state.
     * 
     * @tparam T The type of object to be updated
     * @param id The ID of the object
     * @param state The state of the object to update
     */
    template<class T>
    void update_state(const uint64_t id, const typename T::State state);

    /**
     * @brief Update the state of several existing objects, or add
     *        them if they do not exist.
     * 
     * @tparam T The type of object to update
     * @param objects The list of objects to update
     */
    template<class T>
    void update_all_states(const std::vector<T>& objects);

    /**
     * @brief Update with a list of ids and states only without needing to
     *        create new objects.
     * 
     * @tparam T The type of object to be updated
     * @param ids The list of IDs
     * @param states The list of States
     */
    template<class T>
    void update_all_states
    (
        const std::vector<uint64_t>& ids,
        const typename std::vector<typename T::State>& states
    );

};  // end class CircuitState

}   // end namespace circsim::components

#endif // #ifndef __CIRCSIM_COMPONENTS_CIRCUITSTATE_HPP
