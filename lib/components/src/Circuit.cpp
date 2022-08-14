/**
 * @file Circuit.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Implementation file for the circuit object.
 * @version 0.1
 * @date 2022-08-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <algorithm>
#include <map>
#include <string>
#include <vector>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/IndexError.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>

#include <circsim/components/Circuit.hpp>

using namespace circsim::components;


template<>
std::vector<Transistor>& Circuit::_get_instances<Transistor>()
{
    return _transistor_instances;
}

template<>
std::vector<Wire>& Circuit::_get_instances<Wire>()
{
    return _wire_instances;
}


template<>
std::map<uint64_t, Transistor*>& Circuit::_get_index<Transistor>()
{
    return _transistor_index;
}

template<>
std::map<uint64_t, Wire*>& Circuit::_get_index<Wire>()
{
    return _wire_index;
}


template void Circuit::_index_element<Transistor>(const Transistor& object);
template void Circuit::_index_element<Wire>(const Wire& object);

template<class T>
void Circuit::_index_element(const T& object)
{
    uint64_t id = object.id();
    T* object_ptr = const_cast<T*>(&object);

    if( contains(object) )
    {
        throw circsim::common::IndexError
        (
            "Duplicate ID detected: " + std::to_string(id)
        );
    }

    _get_index<T>()[id] = object_ptr;
}


template void Circuit::_index_all<Transistor>();
template void Circuit::_index_all<Wire>();

template<class T>
void Circuit::_index_all()
{
    _clear_index<T>();

    for( const T& element : _get_instances<T>() )
    {
        _index_element<T>(element);
    }
}


template void Circuit::_clear_index<Transistor>();
template void Circuit::_clear_index<Wire>();

template<class T>
void Circuit::_clear_index()
{
    _get_index<T>().clear();
}


template size_t Circuit::count<Transistor>() const;
template size_t Circuit::count<Wire>() const;

template<class T>
size_t Circuit::count() const
{
    // Override the const value because we're guaranteed not to change anything
    std::vector<T>& storage_array = const_cast<Circuit*>(this)->_get_instances<T>();
    return storage_array.size();
}


template bool Circuit::contains<Transistor>(const Transistor& object) const;
template bool Circuit::contains<Wire>(const Wire& object) const;

template<class T>
bool Circuit::contains(const T& object) const
{
    std::map<uint64_t, T*>& index_map = const_cast<Circuit*>(this)->_get_index<T>();
    typename std::map<uint64_t, T*>::const_iterator it = index_map.find(object.id());

    // Return true only if the key exists and has a non-null value
    return it != index_map.end()
        ? it->second != nullptr
        : false;
}


template bool Circuit::contains_current<Transistor>(const Transistor& object) const;
template bool Circuit::contains_current<Wire>(const Wire& object) const;

template<class T>
bool Circuit::contains_current(const T& object) const
{
    std::map<uint64_t, T*>& index_map = const_cast<Circuit*>(this)->_get_index<T>();
    typename std::map<uint64_t, T*>::const_iterator it = index_map.find(object.id());

    if( (it != index_map.end()) && (it->second != nullptr) )
    {
        return object == *(it->second);
    }

    return false;
}


template void Circuit::add_component<Transistor>(const Transistor& object);
template void Circuit::add_component<Wire>(const Wire& object);

template<class T>
void Circuit::add_component(const T& object)
{
    if( contains(object) )
    {
        throw common::IndexError
        (
            "Database already contains wire with ID: " + std::to_string(object.id())
        );
    }

    // Check if the vector will reallocate
    std::vector<T>& storage_array = _get_instances<T>();
    bool will_realloc =
        storage_array.capacity() == storage_array.size();

    // Note: copy occurs here
    storage_array.push_back(object);

    if( will_realloc )
    {
        _index_all<T>();
    }
    else
    {
        // Note, index from instance list, NOT from supplied parameter
        size_t index = count<T>() - 1;
        _index_element(storage_array[index]);
    }
}


template void Circuit::update_component<Transistor>(const Transistor& object);
template void Circuit::update_component<Wire>(const Wire& object);

template<class T>
void Circuit::update_component(const T& object)
{
    if( contains(object) )
    {
        // Replace the array instance
        std::map<uint64_t, T*>& index_map = _get_index<T>();
        *( index_map[object.id()] ) = object;
    }
    else
    {
        add_component(object);
    }
}


template Transistor* Circuit::get<Transistor>(const uint64_t id) const;
template Wire* Circuit::get<Wire>(const uint64_t id) const;

template<class T>
T* Circuit::get(const uint64_t id) const try
{
    std::map<uint64_t, T*>& index_map = const_cast<Circuit*>(this)->_get_index<T>();
    return index_map.at(id);
}
catch(const std::out_of_range& )
{
    throw common::IndexError
    (
        "Storage does not contain element with ID " + std::to_string(id)
    );
}

template<>
Wire* Circuit::find(const std::string &name) const
{
    auto match_name = [&](const Wire &wire)
    {
        return wire.primary_name() == name;
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
            if( name == other_name )
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
            "Wire not found matching name \"" + name + "\""
        );
    }
}