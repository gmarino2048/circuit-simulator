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
#include <functional>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Library Includes
// (none)

// Project Includes
#include <circsim/common/IndexError.hpp>
#include <circsim/common/ValidationError.hpp>
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
std::vector<Register>& Circuit::_get_instances<Register>()
{
    return _register_instances;
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


Circuit::Circuit(const std::string& name): _name(name) {}


Circuit::Circuit(const Circuit& other)
{
    _name = other._name;

    _transistor_instances = other._transistor_instances;
    _index_all<Transistor>();

    _wire_instances = other._wire_instances;
    _index_all<Wire>();
}


Circuit& Circuit::operator=(const Circuit& other)
{
    if( this == &other )
    {
        return *this;
    }

    _name = other._name;

    _transistor_instances = other._transistor_instances;
    _index_all<Transistor>();

    _wire_instances = other._wire_instances;
    _index_all<Wire>();

    return *this;
}


Circuit::Circuit(Circuit&& other) noexcept
{
    _name = std::move(other._name);

    _transistor_instances = std::move(other._transistor_instances);
    _transistor_index = std::move(other._transistor_index);

    _wire_instances = std::move(other._wire_instances);
    _wire_index = std::move(other._wire_index);
}


Circuit& Circuit::operator=(Circuit&& other) noexcept
{
    if( this == &other )
    {
        return *this;
    }

    _name = std::move(other._name);

    _transistor_instances = std::move(other._transistor_instances);
    _transistor_index = std::move(other._transistor_index);

    _wire_instances = std::move(other._wire_instances);
    _wire_index = std::move(other._wire_index);

    return *this;
}


void Circuit::validate() const
{
    // For each wire, go through the transistor and make
    // sure the transistor has the correct wire id
    std::vector<Wire>::const_iterator wire =
        this->_wire_instances.begin();

    while( wire != this->_wire_instances.end() )
    {
        const uint64_t wire_id = wire->id();

        for( const uint64_t trans_id : wire->ctrl_transistors() )
        {
            Transistor* transistor = nullptr;

            try
            {
                transistor = get<Transistor>(trans_id);
            }
            catch(const circsim::common::IndexError& ex)
            {
                throw circsim::common::ValidationError(ex.what());
            }

            bool correct =
                transistor->source() == wire_id ||
                transistor->drain() == wire_id;

            if( !correct )
            {
                throw circsim::common::ValidationError
                (
                    "Wire with ID \"" + std::to_string(wire_id) + "\" " +
                    "lists control transistor with ID \"" +
                    std::to_string(trans_id) + "\" but transistor does not " +
                    "list wire as source or drain inputs."
                );
            }
        }

        for( const uint64_t trans_id : wire->gate_transistors() )
        {
            Transistor* transistor = nullptr;

            try
            {
                transistor = get<Transistor>(trans_id);
            }
            catch(const circsim::common::IndexError& ex)
            {
                throw circsim::common::ValidationError(ex.what());
            }

            bool correct = transistor->gate() == wire_id;

            if( !correct )
            {
                throw circsim::common::ValidationError
                (
                    "Wire with ID \"" + std::to_string(wire_id) + "\" " +
                    "lists gate transistor with ID \"" +
                    std::to_string(trans_id) + "\" but transistor does not " +
                    "list wire as gate input."
                );
            }
        }

        wire++;
    }

    std::vector<Transistor>::const_iterator transistor =
        this->_transistor_instances.begin();

    while( transistor != this->_transistor_instances.end() )
    {
        const uint64_t transistor_id = transistor->id();

        Wire* source_wire = nullptr;
        Wire* gate_wire = nullptr;
        Wire* drain_wire = nullptr;

        try
        {
            source_wire = get<Wire>(transistor->source());
            gate_wire = get<Wire>(transistor->gate());
            drain_wire = get<Wire>(transistor->drain());
        }
        catch(const circsim::common::IndexError& ex)
        {
            throw circsim::common::ValidationError(ex.what());
        }

        std::vector<uint64_t> source_trans = source_wire->ctrl_transistors();
        std::vector<uint64_t>::const_iterator source_it =
            std::find(source_trans.begin(), source_trans.end(), transistor_id);

        if( source_it == source_trans.end() )
        {
            throw circsim::common::ValidationError
            (
                "Transistor with ID \"" + std::to_string(transistor_id) + "\" " +
                "lists Source Wire \"" + std::to_string(source_wire->id()) + "\" " +
                "but wire does not name transistor in ctrl list."
            );
        }

        std::vector<uint64_t> gate_trans = gate_wire->gate_transistors();
        std::vector<uint64_t>::const_iterator gate_it =
            std::find(gate_trans.begin(), gate_trans.end(), transistor_id);

        if( gate_it == gate_trans.end() )
        {
            throw circsim::common::ValidationError
            (
                "Transistor with ID \"" + std::to_string(transistor_id) + "\" " +
                "lists Gate Wire \"" + std::to_string(gate_wire->id()) + "\" " +
                "but wire does not name transistor in ctrl list."
            );
        }

        std::vector<uint64_t> drain_trans = drain_wire->ctrl_transistors();
        std::vector<uint64_t>::const_iterator drain_it =
            std::find(drain_trans.begin(), drain_trans.end(), transistor_id);

        if( drain_it == drain_trans.end() )
        {
            throw circsim::common::ValidationError
            (
                "Transistor with ID \"" + std::to_string(transistor_id) + "\" " +
                "lists Drain Wire \"" + std::to_string(drain_wire->id()) + "\" " +
                "but wire does not name transistor in ctrl list."
            );
        }

        transistor++;
    }
}


template size_t Circuit::count<Transistor>() const;
template size_t Circuit::count<Wire>() const;
template size_t Circuit::count<Register>() const;

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

template<>
bool Circuit::contains<Register>(const Register& object) const
{
    std::vector<Register>::const_iterator it = std::find_if
    (
        _register_instances.begin(),
        _register_instances.end(),
        [object](const Register& a)
        {
            return object.id() == a.id();
        }
    );

    return it != _register_instances.end();
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

template<>
bool Circuit::contains_current<Register>(const Register& object) const
{
    std::vector<Register>::const_iterator it = std::find_if
    (
        _register_instances.begin(),
        _register_instances.end(),
        [object](const Register& a)
        {
            return object.id() == a.id();
        }
    );

    return it == _register_instances.end()
        ? false
        : object == *it;
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
            "Circuit already contains object with ID: " +
            std::to_string(object.id())
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

template<>
void Circuit::add_component<Register>(const Register& object)
{
    if( contains(object) )
    {
        throw common::IndexError
        (
            "Circuit already contains Register with id: " +
            std::to_string(object.id())
        );
    }

    Register register_copy = object;
    register_copy.set_circuit(*this);

    _register_instances.push_back(register_copy);
}


template void Circuit::add_all_components<Transistor>(const std::vector<Transistor>& object_list);
template void Circuit::add_all_components<Wire>(const std::vector<Wire>& object_list);

template<class T>
void Circuit::add_all_components(const std::vector<T>& object_list)
{
    std::vector<T>& instances = _get_instances<T>();
    instances.reserve(instances.size() + object_list.size());
    _index_all<T>();

    for( const T& object : object_list )
    {
        add_component<T>(object);
    }
}

template<>
void Circuit::add_all_components<Register>(const std::vector<Register>& object_list)
{
    std::vector<Register> registers = object_list;

    for( Register& reg : registers )
    {
        reg.set_circuit(*this);
    }

    std::copy
    (
        registers.begin(),
        registers.end(),
        std::back_inserter(_register_instances)
    );
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

template<>
void Circuit::update_component<Register>(const Register& object)
{
    if( contains(object) )
    {
        std::vector<Register>::iterator it = std::find_if
        (
            _register_instances.begin(),
            _register_instances.end(),
            [object](const Register& a)
            {
                return object.id() == a.id();
            }
        );

        *it = object;
        it->set_circuit(*this);
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
catch( const std::out_of_range& )
{
    throw common::IndexError
    (
        "Storage does not contain element with ID " + std::to_string(id)
    );
}

template<>
Register* Circuit::get(const uint64_t id) const
{
    std::vector<Register>::const_iterator it = std::find_if
    (
        _register_instances.begin(),
        _register_instances.end(),
        [id](const Register& a)
        {
            return id == a.id();
        }
    );

    if( it != _register_instances.end() )
    {
        return const_cast<Register*>(&(*it));
    }
    else
    {
        throw common::IndexError
        (
            "Storage does not contain register with ID " + std::to_string(id)
        );
    }
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