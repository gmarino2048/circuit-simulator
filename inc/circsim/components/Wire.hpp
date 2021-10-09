/**
 * @file Wire.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Contains the declarations for the Wire class. The wires of the
 *        circuit drive the transistors and form the basis for all IO
 *        information. The wires are the most important components, while the
 *        transistors provide switching functionality **only**.
 * @version 0.1
 * @date 2021-09-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CIRCSIM_COMPONENTS_WIRE_HPP
#define __CIRCSIM_COMPONENTS_WIRE_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <functional>

namespace circsim::components {

/**
 * @brief C++ implementation of the wire class. This is **not** intended
 *        to be the lean version of the wire used for calculation, but is
 *        instead the fully-featured version of the wire available for
 *        printing/reading data.
 * 
 */
class Wire final
{
public:

    /**
     * @brief Add an enumerator for special wire types like the VCC and
     *        GND rails.
     * 
     */
    enum SpecialWireType
    {
        VCC,        //!< This is the high voltage rail
        GND,        //!< This is the grounding rail
        CLK         //!< This is the clock signal
    };

    /**
     * @brief One-hot encoded list of potential wire states. The original
     *        circuit simulator library was really inefficient with their
     *        assignment of wire states, so this object allows for simple
     *        value comparison centered around `FLOATING`.
     * 
     */
    enum State : uint8_t
    {
        UNKNOWN =           0b00000000,     //!< Unknown state
        GROUNDED =          0b00000001,     //!< Immutable off state
        PULLED_LOW =        0b00000010,     //!< Definitive off state
        FLOATING_LOW =      0b00000100,     //!< Indeterminate state, currently off
        FLOATING =          0b00001000,     //!< Indeterminate state
        FLOATING_HIGH =     0b00010000,     //!< Indeterminate state, currently on
        PULLED_HIGH =       0b00100000,     //!< Definitive on state
        HIGH =              0b01000000      //!< Immutable on state
    };


private:

    /// The ID of the VCC rail
    static ssize_t _VCC_ID;

    /// The ID of the GND rail
    static ssize_t _GND_ID;

    /// Typedef the optional external driver function so the code is cleaner
    typedef std::optional<std::function<State(State)>> extern_func_t;


    /// The ID of the wire. This will be -1 if it has not been initialized yet
    ssize_t _id;

    /// The name of the wire. This will be empty if the wire has not been initialized
    std::string _primary_name;

    /// Sometimes wires can have alternate names, we should list them here in case
    std::vector<std::string> _other_names;


    /// The current state of the wire, given as one of the above Wire States
    State _state;


    /// Tells whether this wire is potentially externally driven
    bool _externally_driven;

    /// The external function used to drive the wire (if any)
    extern_func_t _driver_function;

    /// The set of transistors which control this wire
    std::vector<size_t> _trans_ctl_ids;

    /// The set of transistors controlled in turn by this wire
    std::vector<size_t> _trans_gate_ids;

public:

    /**
     * @brief Create an empty wire object. This object will have a negative id,
     *        showing that it is not valid. This is just a convenience function for
     *        making a list of pre-allocated Wires.
     * 
     */
    Wire();

    /**
     * @brief Create a special wire from the approved special types. This will use
     *        a premade driver function and is provided for convenience.
     * 
     * @param id                    The id of the wire
     * @param special_type          The premade special type of the wire object
     * @param control_transistors   The list of control transistors for this wire
     * @param gate_transistors      The list of transistors controlled by this wire
     */
    Wire
    (
        const size_t id,
        const SpecialWireType special_type,
        const std::vector<size_t> &control_transistors,
        const std::vector<size_t> &gate_transistors
    );

    /**
     * @brief Create a custom special wire with a specified driver function.
     * 
     * @param id                    The ID of the wire
     * @param name                  The name of the new special wire
     * @param driver_func           The function used to drive the special wire
     * @param control_transistors   The transistors which control this wire
     * @param gate_transistors      The transistors controlled by this wire
     */
    Wire
    (
        const size_t id,
        const std::string &name,
        const std::function<State(State)> driver_func,
        const std::vector<size_t> &control_transistors,
        const std::vector<size_t> &gate_transistors
    );

    /**
     * @brief Create a standard wire used within the processor.
     * 
     * @param id                    The ID of the wire
     * @param name                  The name of the wire
     * @param control_transistors   The transistors which control the wire
     * @param gate_transistors      The transistors controlled by the wire
     * @param initial_state         The initial state of the wire
     */
    Wire
    (
        const size_t id,
        const std::string &name,
        const std::vector<size_t> &control_transistors,
        const std::vector<size_t> &gate_transistors,
        const State initial_state = FLOATING
    );


    /**
     * @brief Get the ID for the VCC Rail
     * 
     * @return size_t The VCC Rail ID
     */
    static size_t VCC_ID();

    /**
     * @brief Get the ID for the GND Rail
     * 
     * @return size_t The GND rail ID
     */
    static size_t GND_ID();


    /**
     * @brief Get this wire's ID. This will be negative if the wire
     *        is not initialized.
     * 
     * @return ssize_t The wire ID
     */
    ssize_t id() const;

    /**
     * @brief Return the primary name of this wire. The primary name is
     *        the first name with which this wire was defined.
     * 
     * @return std::string The primary name
     */
    std::string primary_name() const;

    /**
     * @brief Is this wire driven externally, or by other transistors in the circuit.
     * 
     * @return true     The wire is externally driven
     * @return false    The wire relies on other transistors
     */
    bool externally_driven() const;


    /**
     * @brief Add an additional name to the wire definition. This name will be
     *        added to the other_names list.
     * 
     * @param new_name The new name to add to the wire
     */
    void add_name(const std::string &new_name);

    /**
     * @brief Return the list of other names for this wire.
     * 
     * @return std::vector<std::string> The list of other names for this wire.
     */
    std::vector<std::string> other_names() const;


    /**
     * @brief Return the current state of this wire.
     * 
     * @return State The wire's current state
     */
    State state() const;

    /**
     * @brief Set the state of this wire.
     * 
     * @param new_state The new state to set the wire to
     */
    void state(const State new_state);

    /**
     * @brief Set this wire low.
     * 
     */
    void low() const;

    /**
     * @brief Set this wire high.
     * 
     */
    void high() const;

    /**
     * @brief Convenience method to set the wire high or low.
     * 
     * @param new_state On `true`, set this wire to high. On `false`, set to low.
     */
    void set_high_low(const bool new_state);


    /**
     * @brief Return a string description of this wire.
     * 
     * @return std::string The wire description
     */
    operator std::string();

    /**
     * @brief Compare this wire with another.
     * 
     * @param other The other wire for comparison
     * @return bool `true` if the wires are the same, `false` otherwise
     */
    bool operator==(const Wire &other);

private:

    /**
     * @brief Set the static wire ID parameter when the special wire
     *        is defined.
     * 
     * @param type The type of the special wire
     */
    static void set_special_wire_id(const SpecialWireType type);

    /**
     * @brief Get the name for the special wire.
     * 
     * @param type The type of the special wire
     * @return std::string The special wire's name
     */
    static std::string special_wire_name(const SpecialWireType type);

    /**
     * @brief Get the driver function for a special wire.
     * 
     * @param type The type of the special wire
     * @return extern_func_t The external driver function
     */
    static extern_func_t special_wire_func(const SpecialWireType type);

};

}

#endif//__CIRCSIM_COMPONENTS_WIRE_HPP