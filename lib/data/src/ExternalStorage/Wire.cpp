/**
 * @file Wire.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Definitions for the Wire-Specific implementation of the
 *        ExternalStorage object.
 * @version 0.1
 * @date 2022-08-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
// (none)

// Library Includes
#include <sqlite3.h>

// Project Includes
#include <circsim/components/Wire.hpp>
#include <circsim/data/ExternalStorage.hpp>

using Wire = circsim::components::Wire;
using namespace circsim::data;


template<>
const std::string ExternalStorage::_table_name<Wire>()
{
    return "wires";
}


template<>
void ExternalStorage::_create_table<Wire>()
{
    const std::string query = "CREATE TABLE IF NOT EXISTS " + _table_name<Wire>() + " (" +
        "id INTEGER PRIMARY KEY," +
        "primary_name TEXT NOT NULL," +
        "other_names BLOB NOT NULL," +
        "pulled INTEGER NOT NULL," +
        "control_transistors BLOB NOT NULL," +
        "gate_transistors BLOB NOT NULL" +
        ");";

    sqlite3_stmt* statement = _bind_values(query, {});
    
}