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

class Register final
{
private:

    std::optional<uint64_t> _id;

    std::string _name;

    std::vector<uint64_t> _wire_ids;

    const Circuit* _circuit;

public:

    Register();

    Register(const uint64_t id);

    Register
    (
        const uint64_t id,
        const std::string& name,
        const std::vector<uint64_t>& wire_ids,
    );

    Register
    (
        const uint64_t id,
        const std::string& name,
        const std::vector<uint64_t>& wire_ids,
        const Circuit& circuit_ptr
    );

    inline uint64_t id() try
    {
        return _id.value();
    }
    catch( const std::bad_optional_access& )
    {
        throw circsim::common::ValueError("Register has no ID");
    }

    std::string name() const;

    std::vector<uint64_t> wire_ids() const;


    bool has_circuit() const;

    void set_circuit(const Circuit& circuit);


    template<class T>
    T value_signed() const;

    template<class T>
    void value_signed(const T new_value);

    template<class T>
    T value_unsigned() const;

    template<class T>
    void value_unsigned(const T new_value);

};

}   // end namespace circsim::components

#endif // #ifndef __CIRCSIM_COMPONENTS_REGISTER_HPP
