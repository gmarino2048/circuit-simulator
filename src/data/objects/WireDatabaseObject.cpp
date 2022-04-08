/**
 * @file WireDatabaseObject.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief The definitions file for the Wire Database Object class
 * @version 0.1
 * @date 2022-03-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
// (none)

// Library Includes
#include <sqlite3.h>

// Project includes
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Wire.hpp>
#include <circsim/data/DatabaseObject.hpp>

// Class header
#include <circsim/data/objects/WireDatabaseObject.hpp>

using namespace circsim::components;
using namespace circsim::data;
using namespace circsim::data::objects;

using DbColumn = DatabaseObject::DbColumn;
using DbValue = DatabaseObject::DbValue;

const char *WireDatabaseObject::_wire_table_name = "wires";

const char *WireDatabaseObject::_wire_pkey_col_name = "id";

const std::vector<DbColumn> WireDatabaseObject::_columns = 
{
    {
        .name = "special",
        .type = DbType::DBT_INTEGER
    },
    {
        .name = "primary_name",
        .type = DbType::DBT_TEXT
    },
    {
        .name = "other_names",
        .type = DbType::DBT_TEXT
    },
    {
        .name = "pulled",
        .type = DbType::DBT_INTEGER
    },
    {
        .name = "trans_ctrl_ids",
        .type = DbType::DBT_TEXT
    },
    {
        .name = "trans_gate_ids",
        .type = DbType::DBT_TEXT
    }
};


std::vector<DbColumn> WireDatabaseObject::_get_columns() const
{
    return _columns;
}


DbValue WireDatabaseObject::_get_primary_key() const
{
    // Check for invalid wire object
    if ( _wire_object.id() < 0 )
    {
        throw common::ValueError
        (
            "Invalid wire object when attempting to get primary key"
        );
    }

    size_t id = (size_t) _wire_object.id();
    DbValue value = format_value(id);

    return value;
}


std::vector<DbValue> WireDatabaseObject::_get_values() const
{
    std::vector<DbValue> values;
    values.reserve(_columns.size());

    bool is_special = _wire_object.externally_driven();
    values.push_back(format_value(is_special));

    std::string primary_name = _wire_object.primary_name();
    values.push_back(format_value(primary_name));

    std::vector<std::string> other_names = _wire_object.other_names();
    values.push_back(format_value(other_names));

    size_t pulled = (size_t) _wire_object.pulled_state();
    values.push_back(format_value(pulled));

    std::vector<size_t> ctrl_ids = _wire_object.ctrl_transistors();
    values.push_back(format_value(ctrl_ids));

    std::vector<size_t> gate_ids = _wire_object.gate_transistors();
    values.push_back(format_value(gate_ids));

    return values;
}


WireDatabaseObject::WireDatabaseObject(): DatabaseObject
(
    _columns.size(),
    _wire_table_name,
    _wire_pkey_col_name
)
{
    // Constructor intentionally left blank
}


WireDatabaseObject::WireDatabaseObject(const Wire &wire):
    WireDatabaseObject()
{
    _wire_object = wire;
}


void WireDatabaseObject::import(sqlite3_stmt *statement)
{
    // TODO: Fill this in
}
