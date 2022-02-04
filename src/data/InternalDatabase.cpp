/**
 * @file InternalDatabase.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief This contains the definition for the internal database
 *        instance which is responsible for organizing and indexing
 *        all the components for the simulator
 * @version 0.1
 * @date 2021-11-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <string>

#include <circsim/common/IndexError.hpp>
#include <circsim/data/InternalDatabase.hpp>

/// Convenience typedef for the Wire class
using Wire = circsim::components::Wire;

/// Convenience typedef for the Transistor class
using Transistor = circsim::components::Transistor;

/// Convenience typedef for the InternalDatabase class
using InternalDatabase = circsim::data::InternalDatabase;


void InternalDatabase::_index_element(const Wire &wire)
{
    if( wire.id() < 0 )
    {
        throw common::IndexError
        (
            "Expected nonnegative wire ID for object:\n" +
            static_cast<std::string>(wire)
        );
    }

    size_t id = static_cast<size_t>(wire.id());
    Wire *object_ptr = const_cast<Wire*>(&wire);

    // Ensure that ID is not duplicate
    if( this->contains(wire) )
    {
        throw common::IndexError("Duplicate Wire ID detected: " + std::to_string(wire.id()));
    }

    // No need to do boundary checking since this is addition
    _wire_index[id] = object_ptr;
}


void InternalDatabase::_index_element(const Transistor &transistor)
{
    if( transistor.id() < 0 )
    {
        throw common::IndexError
        (
            "Expected nonnegative transistor ID for object:\n" +
            static_cast<std::string>(transistor)
        );
    }

    size_t id = static_cast<size_t>(transistor.id());
    Transistor *object_ptr = const_cast<Transistor*>(&transistor);

    // Ensure that ID is not duplicate
    if( this->contains(transistor) )
    {
        throw common::IndexError
        (
            "Duplicate Transistor ID detected: " + std::to_string(transistor.id())
        );
    }

    // No need to do boundary checking for set insertion
    _transistor_index[id] = object_ptr;
}


void InternalDatabase::_index_all()
{
    this->_clear_index();

    // Go through transistors first
    for( const Transistor &object : this->_transistor_instances )
    {
        this->_index_element(object);
    }

    // Now go through wires
    for( const Wire &object : this->_wire_instances )
    {
        this->_index_element(object);
    }
}


void InternalDatabase::_clear_index()
{
    this->_transistor_index.clear();
    this->_wire_index.clear();
}


InternalDatabase::InternalDatabase(): 
    _wire_instances(0),
    _transistor_instances(0)
{
    // Default for all other members
}


InternalDatabase::InternalDatabase
(
    const std::vector<Wire>& wires,
    const std::vector<Transistor>& transistors
): _wire_instances(wires),
   _transistor_instances(transistors)
{
    this->_index_all();
}


InternalDatabase::InternalDatabase(const InternalDatabase &other)
{
    // Accelerate by avoiding recalc for self-assignment
    if( this == &other )
    {
        return;
    }

    this->_wire_instances = other._wire_instances;
    this->_transistor_instances = other._transistor_instances;

    this->_index_all();
}


InternalDatabase::InternalDatabase(InternalDatabase &&other) noexcept
{
    // Avoid the self-assignment issue
    if( this == &other )
    {
        return;
    }

    this->_wire_instances = std::move(other._wire_instances);
    this->_transistor_instances = std::move(other._transistor_instances);

    this->_wire_index = std::move(other._wire_index);
    this->_transistor_index = std::move(other._transistor_index);
}


InternalDatabase& InternalDatabase::operator=(const InternalDatabase &other)
{
    if( this == &other )
    {
        return *this;
    }

    this->_wire_instances = other._wire_instances;
    this->_transistor_instances = other._transistor_instances;

    this->_index_all();
    return *this;
}


InternalDatabase& InternalDatabase::operator=(InternalDatabase &&other) noexcept
{
    if( this == &other )
    {
        return *this;
    }

    this->_wire_instances = std::move(other._wire_instances);
    this->_transistor_instances = std::move(other._transistor_instances);

    this->_wire_index = std::move(other._wire_index);
    this->_transistor_index = std::move(other._transistor_index);

    // No need to re-index as wire/transistor pointers should remain the same
    return *this;
}


bool InternalDatabase::contains(const Wire& wire) const
{
    decltype(_wire_index)::const_iterator it = _wire_index.find(wire.id());

    // Return true only if the key exists and has a non-null value
    return it != _wire_index.end()
        ? it->second != nullptr
        : false;
}


bool InternalDatabase::contains(const Transistor& transistor) const
{
    decltype(_transistor_index)::const_iterator it = _transistor_index.find(transistor.id());

    // Return true only if the key exists and has a non-null value
    return it != _transistor_index.end()
        ? it->second != nullptr
        : false;
}


bool InternalDatabase::contains_current(const Wire& wire) const
{
    decltype(_wire_index)::const_iterator it = _wire_index.find(wire.id());

    if( (it != _wire_index.end()) && (it->second != nullptr) )
    {
        return wire == *(it->second);
    }

    return false;
}


bool InternalDatabase::contains_current(const Transistor& transistor) const
{
    decltype(_transistor_index)::const_iterator it = _transistor_index.find(transistor.id());

    if( (it != _transistor_index.end()) && (it->second != nullptr) )
    {
        return transistor == *(it->second);
    }

    return false;
}


void InternalDatabase::add_component(const Wire& wire)
{
    if( contains(wire) )
    {
        throw common::IndexError
        (
            "Database already contains wire with ID: " + std::to_string(wire.id())
        );
    }

    // Check if the vector will reallocate
    bool will_realloc =
        _wire_instances.capacity() == _wire_instances.size();

    // Note: copy occurs here
    _wire_instances.push_back(wire);

    if( will_realloc )
    {
        _index_all();
    }
    else
    {
        // Note, index from instance list, NOT from supplied parameter
        size_t index = wire_count() - 1;
        _index_element(_wire_instances[index]);
    }
}


void InternalDatabase::add_component(const Transistor& transistor)
{
    if( contains(transistor) )
    {
        throw common::IndexError
        (
            "Database already contains transistor with ID: " + std::to_string(transistor.id())
        );
    }

    // Need to check for reallocation
    bool will_realloc = 
        _transistor_instances.capacity() == _transistor_instances.size();

    // Note: copy occurs here
    _transistor_instances.push_back(transistor);

    // If the vector reallocates, we need to index everything again
    if( will_realloc )
    {
        _index_all();
    }
    else
    {
        // Note: need to index the element from the LIST, not the supplied element
        size_t index = transistor_count() - 1;
        _index_element(_transistor_instances[index]);
    }
}


void InternalDatabase::update_component(const Wire& wire)
{
    if( contains(wire) )
    {
        // Replace the array instance
        *( _wire_index[wire.id()] ) = wire;
    }
    else
    {
        add_component(wire);
    }
}


void InternalDatabase::update_component(const Transistor& transistor)
{
    if( contains(transistor) )
    {
        *( _transistor_index[transistor.id()] ) = transistor;
    }
    else
    {
        add_component(transistor);
    }
}


Wire* InternalDatabase::get_wire(const size_t id) const try
{
    return _wire_index.at(id);
}
catch( const std::out_of_range& )
{
    throw common::IndexError
    (
        "Database does not have wire with index " + std::to_string(id)
    );
}


Transistor* InternalDatabase::get_transistor(const size_t id) const try
{
    return _transistor_index.at(id);
}
catch( const std::out_of_range& )
{
    throw common::IndexError
    (
        "Database does not have transistor with index " + std::to_string(id)
    );
}
