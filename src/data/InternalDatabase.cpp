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

using Wire = circsim::components::Wire;
using Transistor = circsim::components::Transistor;
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


InternalDatabase::InternalDatabase
(
    const size_t wire_count,
    const size_t transistor_count
)
{
    if( wire_count != 0 )
    {
        this->_wire_instances.reserve(wire_count);
    }

    if( transistor_count != 0 )
    {
        this->_transistor_instances.reserve(transistor_count);
    }
}
