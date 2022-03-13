/**
 * @file TransistorDatabaseObject.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief The definitions file for the transistor database object
 *        conversion class
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib includes

// Project includes
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/data/DatabaseObject.hpp>

// Header file
#include <circsim/data/objects/TransistorDatabaseObject.hpp>

using namespace circsim::components;
using namespace circsim::data;
using namespace circsim::data::objects;

using DbType = DatabaseObject::DbType;
using DbColumn = DatabaseObject::DbColumn;
using DbValue = DatabaseObject::DbValue;

const char *TransistorDatabaseObject::_transistor_table_name = "transistors";

const char *TransistorDatabaseObject::_transistor_pkey_col_name = "id";

const std::vector<DbColumn> TransistorDatabaseObject::_columns =
{
    {
        .name = "name",
        .type = DbType::DBT_TEXT
    },
    {
        .name = "type",
        .type = DbType::DBT_INTEGER
    },
    {
        .name = "gate_id",
        .type = DbType::DBT_INTEGER
    },
    {
        .name = "src_id",
        .type = DbType::DBT_INTEGER
    },
    {
        .name = "drain_id",
        .type = DbType::DBT_INTEGER
    }
};


std::vector<DbColumn> TransistorDatabaseObject::_get_columns() const
{
    return _columns;
}


DbValue TransistorDatabaseObject::_get_primary_key() const
{
    if ( _transistor_object.id() < 0 )
    {
        throw common::ValueError
        (
            "Invalid transistor ID when attempting to get primary key"
        );
    }

    size_t id = (size_t) _transistor_object.id();
    return format_value(id);
}


std::vector<DbValue> TransistorDatabaseObject::_get_values() const
{
    std::vector<DbValue> values;
    values.reserve(_columns.size());

    // Get transistor name
    std::string name = _transistor_object.name();
    values.push_back(format_value(name));

    // Get transistor type
    size_t type = (size_t) _transistor_object.type();
    values.push_back(format_value(type));

    // Get transistor gate id
    size_t gate_id = _transistor_object.gate();
    values.push_back(format_value(gate_id));

    // Get transistor source id
    size_t source_id = _transistor_object.source();
    values.push_back(format_value(source_id));

    // Get transistor drain id
    size_t drain_id = _transistor_object.drain();
    values.push_back(format_value(drain_id));

    return values;
}


TransistorDatabaseObject::TransistorDatabaseObject() : DatabaseObject
(
    _columns.size(),
    _transistor_table_name,
    _transistor_pkey_col_name
)
{
    // Rest of constructor intentionally blank
}


TransistorDatabaseObject::TransistorDatabaseObject
(
    const Transistor &transistor
) : TransistorDatabaseObject()
{
    _transistor_object = transistor;
}
