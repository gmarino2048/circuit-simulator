/**
 * @file InternalStorage.cpp
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

// C++ Stdlib Includes
#include <algorithm>
#include <string>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/IndexError.hpp>
#include <circsim/data/InternalStorage.hpp>

/// Convenience typedef for the Wire class
using Wire = circsim::components::Wire;

/// Convenience typedef for the Transistor class
using Transistor = circsim::components::Transistor;

/// Convenience typedef for the InternalStorage class
using InternalStorage = circsim::data::InternalStorage;


// Explicit template initialization

// InternalStorage::_index_element
template<>
void InternalStorage::_index_element<Wire>(const Wire& object);

template<>
void InternalStorage::_index_element<Transistor>(const Transistor& object);


// InternalStorage::count
template<>
size_t InternalStorage::count<Wire>() const;

template<>
size_t InternalStorage::count<Transistor>() const;


// InternalStorage::contains
template<>
bool InternalStorage::contains<Wire>(const Wire& object) const;

template<>
bool InternalStorage::contains<Transistor>(const Transistor& object) const;


// InternalStorage::contains_current
template<>
bool InternalStorage::contains_current(const Wire& object) const;

template<>
bool InternalStorage::contains_current(const Transistor& object) const;


// InternalStorage::add_component
template<>
void InternalStorage::add_component(const Wire& object);

template<>
void InternalStorage::add_component(const Transistor& object);


// Method declarations

template<>
void InternalStorage::_index_element(const Wire &object)
{
    if( object.id() < 0 )
    {
        throw common::IndexError
        (
            "Expected nonnegative object ID for object:\n" +
            static_cast<std::string>(object)
        );
    }

    size_t id = static_cast<size_t>(object.id());
    Wire *object_ptr = const_cast<Wire*>(&object);

    // Ensure that ID is not duplicate
    if( this->contains(object) )
    {
        throw common::IndexError("Duplicate Wire ID detected: " + std::to_string(object.id()));
    }

    // No need to do boundary checking since this is addition
    _wire_index[id] = object_ptr;
}


template<>
void InternalStorage::_index_element(const Transistor &object)
{
    if( object.id() < 0 )
    {
        throw common::IndexError
        (
            "Expected nonnegative object ID for object:\n" +
            static_cast<std::string>(object)
        );
    }

    size_t id = static_cast<size_t>(object.id());
    Transistor *object_ptr = const_cast<Transistor*>(&object);

    // Ensure that ID is not duplicate
    if( this->contains(object) )
    {
        throw common::IndexError
        (
            "Duplicate Transistor ID detected: " + std::to_string(object.id())
        );
    }

    // No need to do boundary checking for set insertion
    _transistor_index[id] = object_ptr;
}


void InternalStorage::_index_all()
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


void InternalStorage::_clear_index()
{
    this->_transistor_index.clear();
    this->_wire_index.clear();
}


InternalStorage::InternalStorage(): 
    _wire_instances(0),
    _transistor_instances(0)
{
    // Default for all other members
}


InternalStorage::InternalStorage
(
    const std::vector<Wire>& wires,
    const std::vector<Transistor>& transistors
): _wire_instances(wires),
   _transistor_instances(transistors)
{
    this->_index_all();
}


InternalStorage::InternalStorage(const InternalStorage &other)
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


InternalStorage::InternalStorage(InternalStorage &&other) noexcept
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


InternalStorage& InternalStorage::operator=(const InternalStorage &other)
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


InternalStorage& InternalStorage::operator=(InternalStorage &&other) noexcept
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


template<>
size_t InternalStorage::count<Wire>() const
{
    return this->_wire_instances.size();
}


template<>
size_t InternalStorage::count<Transistor>() const
{
    return this->_transistor_instances.size();
}


template<>
bool InternalStorage::contains(const Wire& object) const
{
    decltype(_wire_index)::const_iterator it = _wire_index.find(object.id());

    // Return true only if the key exists and has a non-null value
    return it != _wire_index.end()
        ? it->second != nullptr
        : false;
}


template<>
bool InternalStorage::contains(const Transistor& object) const
{
    decltype(_transistor_index)::const_iterator it = _transistor_index.find(object.id());

    // Return true only if the key exists and has a non-null value
    return it != _transistor_index.end()
        ? it->second != nullptr
        : false;
}


template<>
bool InternalStorage::contains_current(const Wire& object) const
{
    decltype(_wire_index)::const_iterator it = _wire_index.find(object.id());

    if( (it != _wire_index.end()) && (it->second != nullptr) )
    {
        return object == *(it->second);
    }

    return false;
}


template<>
bool InternalStorage::contains_current(const Transistor& object) const
{
    decltype(_transistor_index)::const_iterator it = _transistor_index.find(object.id());

    if( (it != _transistor_index.end()) && (it->second != nullptr) )
    {
        return object == *(it->second);
    }

    return false;
}


template<>
void InternalStorage::add_component(const Wire& object)
{
    if( contains(object) )
    {
        throw common::IndexError
        (
            "Database already contains wire with ID: " + std::to_string(object.id())
        );
    }

    // Check if the vector will reallocate
    bool will_realloc =
        _wire_instances.capacity() == _wire_instances.size();

    // Note: copy occurs here
    _wire_instances.push_back(object);

    if( will_realloc )
    {
        _index_all();
    }
    else
    {
        // Note, index from instance list, NOT from supplied parameter
        size_t index = count<Wire>() - 1;
        _index_element(_wire_instances[index]);
    }
}


template<>
void InternalStorage::add_component(const Transistor& object)
{
    if( contains(object) )
    {
        throw common::IndexError
        (
            "Database already contains transistor with ID: " + std::to_string(object.id())
        );
    }

    // Need to check for reallocation
    bool will_realloc = 
        _transistor_instances.capacity() == _transistor_instances.size();

    // Note: copy occurs here
    _transistor_instances.push_back(object);

    // If the vector reallocates, we need to index everything again
    if( will_realloc )
    {
        _index_all();
    }
    else
    {
        // Note: need to index the element from the LIST, not the supplied element
        size_t index = count<Transistor>() - 1;
        _index_element(_transistor_instances[index]);
    }
}


void InternalStorage::update_component(const Wire& wire)
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


void InternalStorage::update_component(const Transistor& transistor)
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


Wire* InternalStorage::get_wire(const size_t id) const try
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


Transistor* InternalStorage::get_transistor(const size_t id) const try
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


Wire* InternalStorage::find_wire(const std::string &wire_name) const
{
    auto match_name = [&](const Wire &wire)
    {
        return wire.primary_name() == wire_name;
    };

    std::vector<Wire>::const_iterator wire_found;
    wire_found = std::find_if
    (
        _wire_instances.begin(),
        _wire_instances.end(),
        match_name
    );

    if ( wire_found != _wire_instances.end())
    {
        return const_cast<Wire*>(&(*wire_found));
    }

    auto match_other_names = [&](const Wire &wire)
    {
        for( const std::string &other_name : wire.other_names() )
        {
            if( wire_name == other_name )
            {
                return true;
            }
        }

        return false;
    };

    wire_found = std::find_if
    (
        _wire_instances.begin(),
        _wire_instances.end(),
        match_other_names
    );

    if ( wire_found != _wire_instances.end())
    {
        return const_cast<Wire*>(&(*wire_found));
    }
    else
    {
        throw common::IndexError
        (
            "Wire not found matching name \"" + wire_name + "\""
        );
    }
}
