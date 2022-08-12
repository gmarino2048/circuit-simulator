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
    using ObjectState = std::pair<uint64_t, T::State>;

private:

    /// The ID of this object. This is not to be confused with its timestamp.
    std::optional<uint64_t> _id;

    /// A list of the states of all transistors registered in this object.
    std::vector<ObjectState<Transistor>> _transistor_states;

    /// A list of the states of all wires registered in this object.
    std::vector<ObjectState<Wire>> _wire_state;


    /**
     * @brief Gets the state list for the associated object.
     * 
     * @tparam T The type of the object
     * @return std::vector<ObjectState<T>>& A reference to the state list
     */
    template<class T>
    std::vector<ObjectState<T>>& _get_state_list() const;

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
     * @brief Create a new CircuitState object and populate with provided values.
     * 
     * @param id The ID of the newly created object
     * @param transistor_list A list of transistors to take a snapshot with
     * @param wire_list A list of wires to take a snapshot with
     */
    CircuitState
    (
        const uint64_t id,
        const std::vector<Transistor>& transistor_list,
        const std::vector<Wire>& wire_list
    );


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
     * @brief Update the state of several existing objects, or add
     *        them if they do not exist.
     * 
     * @tparam T The type of object to update
     * @param objects The list of objects to update
     */
    template<class T>
    void update_all_states(const std::vector<T>& objects);

};  // end class CircuitState

}   // end namespace circsim::components

#endif // #ifndef __CIRCSIM_COMPONENTS_CIRCUITSTATE_HPP
