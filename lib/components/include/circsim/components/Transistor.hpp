/**
 * @file Transistor.hpp
 * @author Guy Marino (gxm262@case.edu)
 * @brief This file contains the header definition for the
 *        base transistor class.
 * @version 0.1
 * @date 2021-10-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CIRCSIM_COMPONENTS_TRANSISTOR_HPP
#define __CIRCSIM_COMPONENTS_TRANSISTOR_HPP

// C++ Stdlib Includes
#include <cstdint>
#include <string>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Wire.hpp>

namespace circsim::components
{

/**
 * @brief This is the base class for the Transistor class. This can
 *        then be converted into N-Type and P-Type transistors with
 *        differentiated behavior.
 * 
 */
class Transistor final {
public:

    /// Easily get the wire state type
    typedef Wire::State WireState;

    /// The switching type of this transistor object
    enum Type : uint8_t
    {
        NMOS        = 0,    //!< Gate=HIGH means on
        PMOS        = 1     //!< Gate=LOW means on
    };

    /// The current state of this transistor
    enum State : uint8_t
    {
        ON,         //!< The transistor is currently a closed circuit
        OFF         //!< The transistor is currently an open circuit
    };

private:

    /// The ID of this transistor
    std::optional<uint64_t> _id;

    /// The name of this transistor, which can be empty if it is unnamed
    std::string _name;

    /// The type of the transistor, either NMOS or PMOS
    Type _type;


    /// The ID of the gate wire for the transistor
    uint64_t _gate_id;

    /// The ID of the source wire for the transistor
    uint64_t _source_id;

    /// The ID of the drain wire for the transistor
    uint64_t _drain_id;


    /// Whether the transistor has been inititalized
    bool _initialized;

    /// The current state of the transistor
    State _current_state;


    /**
     * @brief Update the current state of the transistor given the
     *        gate state, given the transistor behaves as an NMOS
     *        transistor.
     * 
     * @param gate_state The new gate state to recalculate from
     * @return true The transistor state has changed
     * @return false The transistor state has not changed
     */
    bool _update_nmos(const WireState gate_state);

    /**
     * @brief Update the current state of the transistor given the
     *        gate state, given the transistor behaves as a PMOS
     *        transistor.
     * 
     * @param gate_state The new gate state to recalculate from
     * @return true The transistor state has changed
     * @return false The transistor state has not changed
     */
    bool _update_pmos(const WireState gate_state);

public:

    /**
     * @brief Construct a new empty Transistor object
     * 
     */
    Transistor();

    /**
     * @brief Construct a new Transistor object
     * 
     * @param id            The ID of the transistor
     * @param gate_id       The gate wire ID
     * @param source_id     The source wire ID
     * @param drain_id      The drain wire ID
     * @param type          The type of the transistor
     */
    Transistor(
        const uint64_t id,
        const uint64_t gate_id,
        const uint64_t source_id,
        const uint64_t drain_id,
        const Type type = Type::NMOS
    );

    /**
     * @brief Construct a new Transistor object
     * 
     * @param id            The ID of the transistor
     * @param name          The name of the transistor
     * @param gate_id       The gate wire ID
     * @param source_id     The source wire ID
     * @param drain_id      The drain wire ID
     * @param type          The type of transistor
     */
    Transistor(
        const uint64_t id,
        const std::string &name,
        const uint64_t gate_id,
        const uint64_t source_id,
        const uint64_t drain_id,
        const Type type = Type::NMOS
    );


    /**
     * @brief Return the ID of this transistor
     * 
     * @return uint64_t The ID of the transistor
     */
    inline uint64_t id() const try
    { 
        return _id.value();
    }
    catch( const std::bad_optional_access& )
    {
        throw circsim::common::ValueError("Transistor currently has no ID");
    }

    /**
     * @brief Return the name of this transistor
     * 
     * @return std::string The name of the transistor
     */
    inline std::string name() const noexcept { return _name; }

    /**
     * @brief Get the type of the transistor.
     * 
     * @return Type The type of this transistor.
     */
    inline Type type() const noexcept { return _type; }


    /**
     * @brief Get the gate wire ID.
     * 
     * @return size_t The gate wire ID.
     */
    inline size_t gate() const noexcept { return _gate_id; }

    /**
     * @brief Get the source wire ID.
     * 
     * @return size_t The source wire ID.
     */
    inline size_t source() const noexcept { return _source_id; }

    /**
     * @brief Get the drain wire ID.
     * 
     * @return size_t The drain wire ID.
     */
    inline size_t drain() const noexcept { return _drain_id; }

    /**
     * @brief Returns the initialization state of this transistor.
     * 
     * If the object has not been initialized, we can't use state optimization
     * when running the simulation since wires might not get updated properly.
     * 
     * @return true if the transistor has already been initialized, false otherwise.
     */
    inline bool is_initialized() const noexcept { return _initialized; }

    /**
     * @brief Mark this transistor as initialized for optimized simulator behavior.
     * 
     */
    inline void set_initialized() noexcept { _initialized = true; }

    /**
     * @brief Get the current state of the transistor.
     * 
     * @return State The current state.
     */
    inline State current_state() const noexcept { return _current_state; }

    /**
     * @brief Update the transistor state given the current state of the
     *        gate.
     * 
     * @param gate_state The state of the gate wire.
     * @return true The transistor value has changed.
     * @return false The transistor value has not changed.
     */
    bool update_state(const WireState gate_state);

    /**
     * @brief Equality operator. Compares the internal components of two
     *        transistors.
     * 
     * @param rhs The other transistor to compare with.
     * @return bool Whether the transistors are equivalent.
     */
    bool operator==(const Transistor& rhs) const;


    /**
     * @brief Conversion from transistor to string instance
     * 
     * @return std::string A printable representation of the transistor
     */
    operator std::string() const;

};

}

#endif//__CIRCSIM_COMPONENTS_TRANSISTOR_HPP