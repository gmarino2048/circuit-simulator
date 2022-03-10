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
    return std::vector<DbValue>();
}
