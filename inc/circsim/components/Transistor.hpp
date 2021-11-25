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

#include <string>
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
    enum Type
    {
        NMOS,           //!< Gate=HIGH means on
        PMOS            //!< Gate=LOW means on
    };

    /// The current state of this transistor
    enum State
    {
        ON,         //!< The transistor is currently a closed circuit
        OFF         //!< The transistor is currently an open circuit
    };

private:

    /// The ID of this transistor
    ssize_t _id;

    /// The name of this transistor, which can be empty if it is unnamed
    std::string _name;

    /// The type of the transistor, either NMOS or PMOS
    Type _type;


    /// The ID of the gate wire for the transistor
    size_t _gate_id;

    /// The ID of the source wire for the transistor
    size_t _source_id;

    /// The ID of the drain wire for the transistor
    size_t _drain_id;


    /// The current state of the transistor
    State _current_state;

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
     */
    Transistor(
        const size_t id,
        const size_t gate_id,
        const size_t source_id,
        const size_t drain_id
    );

    /**
     * @brief Construct a new Transistor object
     * 
     * @param id            The ID of the transistor
     * @param name          The name of the transistor
     * @param gate_id       The gate wire ID
     * @param source_id     The source wire ID
     * @param drain_id      The drain wire ID
     */
    Transistor(
        const size_t id,
        const std::string &name,
        const size_t gate_id,
        const size_t source_id,
        const size_t drain_id
    );


    /**
     * @brief Return the ID of this transistor
     * 
     * @return ssize_t The ID of the transistor
     */
    ssize_t id() const noexcept;

    /**
     * @brief Return the name of this transistor
     * 
     * @return std::string The name of the transistor
     */
    std::string name() const noexcept;

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