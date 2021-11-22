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
        throw std::runtime_error
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
        throw IndexError("Duplicate Wire ID detected: " + wire.id());
    }

    // No need to do boundary checking since this is addition
    _wire_index[id] = object_ptr;
}


void InternalDatabase::_index_element(const Transistor &transistor)
{
    if( transistor.id() < 0 )
    {
        throw std::runtime_error
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
        throw IndexError("Duplicate Transistor ID detected: " + transistor.id());
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
