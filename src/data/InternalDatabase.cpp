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
