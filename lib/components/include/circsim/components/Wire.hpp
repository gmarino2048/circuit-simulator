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

// C++ Stdlib Includes
#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <functional>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/ValueError.hpp>

namespace circsim::components
{

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
        GND         //!< This is the grounding rail
    };

    /**
     * @brief The pullup/pulldown connection status of the wire
     * 
     */
    enum PulledStatus : uint8_t
    {
        PS_NONE         = 0b00,     //!< The wire is not connected to a pullup/pulldown
        PS_HIGH         = 0b01,     //!< The wire is connected to a pullup
        PS_LOW          = 0b10      //!< The wire is connected to a pulldown
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
        GROUNDED =          0b00000001,     //!< Immutable off state
        PULLED_LOW =        0b00000010,     //!< Definitive off state
        FLOATING_LOW =      0b00000100,     //!< Indeterminate state, currently off
        FLOATING =          0b00001000,     //!< Indeterminate state
        FLOATING_HIGH =     0b00010000,     //!< Indeterminate state, currently on
        PULLED_HIGH =       0b00100000,     //!< Definitive on state
        HIGH =              0b01000000      //!< Immutable on state
    };


private:

    /// Typedef the optional external driver function so the code is cleaner
    typedef std::optional<std::function<State(State)>> extern_func_t;


    /// The ID of the VCC rail
    static std::optional<uint64_t> _VCC_ID;

    /// The ID of the GND rail
    static std::optional<uint64_t> _GND_ID;


    /// The VCC rail state function
    static const std::function<State(State)> _VCC_FUNC;

    /// The GND rail state function
    static const std::function<State(State)> _GND_FUNC;


    /// The ID of the wire. This will be -1 if it has not been initialized yet
    std::optional<uint64_t> _id;

    /// The name of the wire. This will be empty if the wire has not been initialized
    std::string _primary_name;

    /// Sometimes wires can have alternate names, we should list them here in case
    std::vector<std::string> _other_names;


    /// Tells whether this wire is connected to a pullup/pulldown
    PulledStatus _pulled;

    /// The current state of the wire, given as one of the above Wire States
    State _state = State::FLOATING;


    /// The set of transistors which control this wire
    std::vector<uint64_t> _trans_ctl_ids;

    /// The set of transistors controlled in turn by this wire
    std::vector<uint64_t> _trans_gate_ids;

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
        const uint64_t id,
        const SpecialWireType special_type,
        const std::vector<uint64_t> &control_transistors,
        const std::vector<uint64_t> &gate_transistors
    );

    /**
     * @brief Create a standard wire used within the processor.
     * 
     * @param id                    The ID of the wire
     * @param name                  The name of the wire
     * @param pulled                The pullup/pulldown status of the wire
     * @param control_transistors   The transistors which control the wire
     * @param gate_transistors      The transistors controlled by the wire
     */
    Wire
    (
        const uint64_t id,
        const std::string &name,
        const PulledStatus pulled,
        const std::vector<uint64_t> &control_transistors,
        const std::vector<uint64_t> &gate_transistors
    );


    /**
     * @brief Returns whether the VCC wire currently has an assigned value.
     * 
     * @return true There is a VCC wire in the circuit
     * @return false There is no VCC wire in the circuit
     */
    static inline bool VCC_ID_EXISTS() { return _VCC_ID.has_value(); }

    /**
     * @brief Get the ID for the VCC Rail
     * 
     * @return uint64_t The VCC Rail ID
     */
    static inline uint64_t VCC_ID() try
    {
        return _VCC_ID.value();
    }
    catch( const std::bad_optional_access& )
    {
        throw circsim::common::ValueError("VCC_ID is not currently set.");
    }


    /**
     * @brief Returns whether the GND wire currently has an assigned value.
     * 
     * @return true There is a GND wire in the circuit
     * @return false There is no GND wire in the circuit
     */
    static inline bool GND_ID_EXISTS() { return _GND_ID.has_value(); }

    /**
     * @brief Get the ID for the GND Rail
     * 
     * @return uint64_t The GND rail ID
     */
    static inline uint64_t GND_ID() try
    {
        return _GND_ID.value();
    }
    catch( const std::bad_optional_access& )
    {
        throw circsim::common::ValueError("GND_ID is not currently set.");
    }

    /**
     * @brief Reset the class instance to its original state.
     * 
     */
    static void RESET_CLASS();


    /**
     * @brief Get this wire's ID. This will be negative if the wire
     *        is not initialized.
     * 
     * @return uint64_t The wire ID
     */
    inline uint64_t id() const try
    { 
        return _id.value();
    }
    catch( const std::bad_optional_access& )
    {
        throw circsim::common::ValueError("Wire object currently has no ID");
    }

    /**
     * @brief Return the primary name of this wire. The primary name is
     *        the first name with which this wire was defined.
     * 
     * @return std::string The primary name
     */
    inline std::string primary_name() const noexcept { return _primary_name; }

    /**
     * @brief Return the list of control transistors for this object.
     * 
     * @return std::vector<uint64_t> The IDs of the control transistors.
     */
    inline std::vector<uint64_t> ctrl_transistors() const noexcept { return _trans_ctl_ids; }

    /**
     * @brief Return the list of gate transistors for this wire.
     * 
     * @return std::vector<uint64_t> The IDs of the gate transistor.
     */
    inline std::vector<uint64_t> gate_transistors() const noexcept { return _trans_gate_ids; }


    /**
     * @brief Returns whether this wire is a special wire.
     * 
     * @return true if the wire is VCC or GND, false otherwise.
     */
    bool special() const;


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
    inline std::vector<std::string> other_names() const noexcept { return _other_names; }


    /**
     * @brief Return the current state of this wire.
     * 
     * @return State The wire's current state
     */
    inline State state() const noexcept { return _state; }

    /**
     * @brief Set the state of this wire.
     * 
     * @param new_state The new state to set the wire to
     */
    inline void state(const State new_state) noexcept { _state = new_state; }


    /**
     * @brief Return whether this method is a pullup or pulldown.
     * 
     * @return bool Whether the wire is pulled or not
     */
    bool pulled() const noexcept;


    /**
     * @brief Determines if the wire has been pulled high or low
     * 
     * @return PulledStatus whether the wire is pulled high, low, or neither
     */
    inline PulledStatus pulled_state() const noexcept { return _pulled; }

    /**
     * @brief Returns whether this wire is low
     * 
     * @return true The current wire is low
     * @return false The current wire is high
     */
    bool low() const noexcept;

    /**
     * @brief Returns whether this wire is high
     * 
     * @return true The current wire is high
     * @return false The current wire is low
     */
    bool high() const noexcept;

    /**
     * @brief Set the wire to its floating state as
     *        determined by the pullup/pulldown state
     * 
     */
    void set_floating();

    /**
     * @brief Convenience method to set the wire high or low.
     * 
     * @param new_state On `true`, set this wire to high. On `false`, set to low.
     */
    void set_high_low(const bool new_state) noexcept;


    /**
     * @brief Return a string description of this wire.
     * 
     * @return std::string The wire description
     */
    operator std::string() const;

    /**
     * @brief Compare this wire with another.
     * 
     * @param rhs The other wire for comparison
     * @return bool `true` if the wires are the same, `false` otherwise
     */
    bool operator==(const Wire &rhs) const;

private:

    /**
     * @brief Set the static wire ID parameter when the special wire
     *        is defined.
     * 
     * @param type The type of the special wire
     */
    void set_special_wire_id(const SpecialWireType type);

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


    /**
     * @brief Convert the wire's current state to a readable string
     *        representation.
     * 
     * @param state The wire state
     * @return std::string A string representation of the state
     */
    static std::string state_to_string(const State state);

};

}

#endif//__CIRCSIM_COMPONENTS_WIRE_HPP