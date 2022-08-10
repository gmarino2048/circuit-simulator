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
#include <circsim/common/StateError.hpp>
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Wire.hpp>
#include <circsim/data/ExternalStorage.hpp>

using Wire = circsim::components::Wire;
using namespace circsim::data;


template<>
const std::string ExternalStorage::_table_name<Wire>()
{
    return "wires";
}


const size_t WIRE_FIELD_COUNT = 6;

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
    int result = sqlite3_step(statement);

    if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(_db_connection_obj)
        );
    }

    sqlite3_finalize(statement);
}


template<>
std::vector<ExternalStorage::SqlValue> ExternalStorage::_encode(const Wire& object) const
{
    std::vector<SqlValue> values;
    values.reserve(WIRE_FIELD_COUNT);

    values.push_back(_to_sql_type<uint64_t>(object.id()));
    values.push_back(_to_sql_type<std::string>(object.primary_name()));
    values.push_back(_to_sql_type<std::vector<std::string>>(object.other_names()));
    values.push_back(_to_sql_type<uint8_t>(object.pulled_state()));
    values.push_back(_to_sql_type<std::vector<uint64_t>>(object.ctrl_transistors()));
    values.push_back(_to_sql_type<std::vector<uint64_t>>(object.gate_transistors()));

    return values;
}


template<>
Wire ExternalStorage::_decode(const std::vector<SqlValue>& values) const
{
    if( values.size() != WIRE_FIELD_COUNT )
    {
        throw circsim::common::ValueError
        (
            "Field count mismatch when attempting to decode wire"
        );
    }

    uint64_t id = _from_sql_type<uint64_t>(values[0]);
    std::string name = _from_sql_type<std::string>(values[1]);
    std::vector<std::string> other_names = _from_sql_type<std::vector<std::string>>(values[2]);
    Wire::PulledStatus pulled = static_cast<Wire::PulledStatus>(_from_sql_type<uint8_t>(values[3]));
    std::vector<uint64_t> ctrl_ids = _from_sql_type<std::vector<uint64_t>>(values[4]);
    std::vector<uint64_t> gate_ids = _from_sql_type<std::vector<uint64_t>>(values[5]);

    Wire wire
    (
        id,
        name,
        pulled,
        ctrl_ids,
        gate_ids
    );

    for( const std::string& other_name : other_names )
    {
        wire.add_name(other_name);
    }

    return wire;
}


template<>
bool ExternalStorage::contains(const Wire& object) const
{

}
