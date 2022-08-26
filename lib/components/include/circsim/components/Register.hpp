/**
 * @file Register.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Emulate a register value using a series of pulled
 *        wires. This object supports retriving and setting
 *        register values as integers.
 * @version 0.1
 * @date 2022-08-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_COMPONENTS_REGISTER_HPP
#define __CIRCSIM_COMPONENTS_REGISTER_HPP

// C++ Stdlib Includes
#include <cinttypes>
#include <string>
#include <optional>
#include <vector>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Circuit.hpp>
#include <circsim/components/Wire.hpp>


namespace circsim::components
{

/**
 * @brief This class contains a definition of the optional
 *        Register add-on component, which can be used to view
 *        and modify several wire values according to the
 *        provided integer value.
 * 
 * The list of wires must be provided in little-endian bit order in
 * order to work correctly. This object uses a mathematical conversion
 * process, and may not accurately reflect the value seen by the target
 * circuit.
 * 
 */
class Register final
{
private:

    /// The ID of this register
    std::optional<uint64_t> _id;

    /// The name of this register
    std::string _name;

    /// The little-endian list of wire IDs to use for the
    /// register value
    std::vector<uint64_t> _wire_ids;

    /// The reference to the circuit object used for value lookup
    const Circuit* _circuit;

public:

    /**
     * @brief Default constructor.
     * 
     */
    Register();

    /**
     * @brief Construct a new Register object without Wire state
     *        lookup.
     * 
     * @param id The ID of this register object
     * @param name The name of this register object
     * @param wire_ids The little-endian list of wire IDs for this register
     */
    Register
    (
        const uint64_t id,
        const std::string& name,
        const std::vector<uint64_t>& wire_ids
    );

    /**
     * @brief Construct a new Register object with Wire State lookup.
     * 
     * @param id The ID of this register object
     * @param name The name of this register object
     * @param wire_ids The little-endian list of wire IDs for this register
     * @param circuit_ptr The circuit reference used for state lookup.
     */
    Register
    (
        const uint64_t id,
        const std::string& name,
        const std::vector<uint64_t>& wire_ids,
        const Circuit& circuit
    );


    /**
     * @brief Get the ID of this register, and throws an error if
     *        the value doesn't exist.
     * 
     * @return uint64_t The ID of this register
     */
    inline uint64_t id() try
    {
        return _id.value();
    }
    catch( const std::bad_optional_access& )
    {
        throw circsim::common::ValueError("Register has no ID");
    }

    /**
     * @brief Gets the name of this register.
     * 
     * @return std::string The name of the register
     */
    inline std::string name() const { return _name; }

    /**
     * @brief Gets a copy of the little-endian list of wire IDs
     *        for this register.
     * 
     * @return std::vector<uint64_t> The little-endian wire ids
     */
    inline std::vector<uint64_t> wire_ids() const { return _wire_ids; }


    /**
     * @brief Returns whether this register has an associated Circuit
     *        object for wire state lookup.
     * 
     * @return bool Whether this register has a circuit reference.
     */
    bool has_circuit() const;

    /**
     * @brief Set the Circuit object used for looking up
     *        wire states.
     * 
     * @param circuit The new circuit object for lookup
     */
    void set_circuit(const Circuit& circuit);

    /**
     * @brief Clear this register's lookup circuit.
     * 
     */
    void clear_circuit();


    /**
     * @brief Get the two's complement signed value of this
     *        register's current configuration.
     * 
     * @tparam T The integer type to return
     * @return T The signed value of the register
     */
    template<class T>
    T value_signed() const;

    /**
     * @brief Set the value of this register to the given
     *        signed value.
     * 
     * @tparam T The integer type used to set the register
     * @param new_value The value to set the register
     */
    template<class T>
    void value_signed(const T new_value);

    /**
     * @brief Get the raw unsigned value of this register.
     * 
     * @tparam T The integer type to return
     * @return T The current unsigned value of the register
     */
    template<class T>
    T value_unsigned() const;

    /**
     * @brief Set the unsigned value of this register.
     * 
     * @tparam T The integer type used to set the register
     * @param new_value The value to set the register
     */
    template<class T>
    void value_unsigned(const T new_value);

};

}   // end namespace circsim::components

#endif // #ifndef __CIRCSIM_COMPONENTS_REGISTER_HPP
