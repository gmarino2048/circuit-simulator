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

// InternalStorage::_get_storage
template<>
std::vector<Wire>& InternalStorage::_get_storage();

template<>
std::vector<Transistor>& InternalStorage::_get_storage();


// InternalStorage::_get_index
template<>
std::map<size_t, Wire*>& InternalStorage::_get_index();

template<>
std::map<size_t, Transistor*>& InternalStorage::_get_index();


// InternalStorage::_get_typename
template<>
const std::string InternalStorage::_get_typename<Wire>() const;

template<>
const std::string InternalStorage::_get_typename<Transistor>() const;


// InternalStorage::_index_element
template void InternalStorage::_index_element<Wire>(const Wire& object);
template void InternalStorage::_index_element<Transistor>(const Transistor& object);


// InternalStorage::count
template size_t InternalStorage::count<Wire>() const;
template size_t InternalStorage::count<Transistor>() const;


// InternalStorage::contains
template bool InternalStorage::contains<Wire>(const Wire& object) const;
template bool InternalStorage::contains<Transistor>(const Transistor& object) const;


// InternalStorage::contains_current
template bool InternalStorage::contains_current(const Wire& object) const;
template bool InternalStorage::contains_current(const Transistor& object) const;


// InternalStorage::add_component
template void InternalStorage::add_component(const Wire& object);
template void InternalStorage::add_component(const Transistor& object);


// InternalStorage::update_component
template void InternalStorage::update_component(const Wire& object);
template void InternalStorage::update_component(const Transistor& object);


// Method declarations

template<>
std::vector<Wire>& InternalStorage::_get_storage()
{
    return this->_wire_instances;
}


template<>
std::vector<Transistor>& InternalStorage::_get_storage()
{
    return this->_transistor_instances;
}


template<>
std::map<size_t, Wire*>& InternalStorage::_get_index()
{
    return this->_wire_index;
}


template<>
std::map<size_t, Transistor*>& InternalStorage::_get_index()
{
    return this->_transistor_index;
}


template<>
const std::string InternalStorage::_get_typename<Wire>() const
{
    return "Wire";
}

template<>
const std::string InternalStorage::_get_typename<Transistor>() const
{
    return "Transistor";
}


template<class T>
void InternalStorage::_index_element(const T& object)
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
    T* object_ptr = const_cast<T*>(&object);

    // Ensure that ID is not duplicate
    if( this->contains(object) )
    {
        std::string errmsg = "Duplicate " + _get_typename<T>() + " ID detected: ";
        throw common::IndexError(errmsg + std::to_string(object.id()));
    }

    // No need to do boundary checking since this is addition
    _get_index<T>()[id] = object_ptr;
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


template<class T>
size_t InternalStorage::count() const
{
    // Override the const value because we're guaranteed not to change anything
    std::vector<T>& storage_array = const_cast<InternalStorage*>(this)->_get_storage<T>();
    return storage_array.size();
}


template<class T>
bool InternalStorage::contains(const T& object) const
{
    std::map<size_t, T*>& index_map = const_cast<InternalStorage*>(this)->_get_index<T>();
    typename std::map<size_t, T*>::const_iterator it = index_map.find(object.id());

    // Return true only if the key exists and has a non-null value
    return it != index_map.end()
        ? it->second != nullptr
        : false;
}


template<class T>
bool InternalStorage::contains_current(const T& object) const
{
    std::map<size_t, T*>& index_map = const_cast<InternalStorage*>(this)->_get_index<T>();
    typename std::map<size_t, T*>::const_iterator it = index_map.find(object.id());

    if( (it != index_map.end()) && (it->second != nullptr) )
    {
        return object == *(it->second);
    }

    return false;
}


template<class T>
void InternalStorage::add_component(const T& object)
{
    if( contains(object) )
    {
        throw common::IndexError
        (
            "Database already contains wire with ID: " + std::to_string(object.id())
        );
    }

    // Check if the vector will reallocate
    std::vector<T>& storage_array = _get_storage<T>();
    bool will_realloc =
        storage_array.capacity() == storage_array.size();

    // Note: copy occurs here
    storage_array.push_back(object);

    if( will_realloc )
    {
        _index_all();
    }
    else
    {
        // Note, index from instance list, NOT from supplied parameter
        size_t index = count<T>() - 1;
        _index_element(storage_array[index]);
    }
}


template<class T>
void InternalStorage::update_component(const T& object)
{
    if( contains(object) )
    {
        // Replace the array instance
        std::map<size_t, T*>& index_map = _get_index<T>();
        *( index_map[object.id()] ) = object;
    }
    else
    {
        add_component(object);
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
