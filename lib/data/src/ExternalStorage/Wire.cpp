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
#include <cstring>

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
        "other_names BLOB," +
        "pulled INTEGER NOT NULL," +
        "control_transistors BLOB," +
        "gate_transistors BLOB" +
    ");";

    SqliteStatement statement = _bind_values(query, {});
    int result = sqlite3_step(statement);

    if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(_db_connection_obj)
        );
    }
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
Wire ExternalStorage::_decode(SqliteStatement& statement) const
{
    int name_size = sqlite3_column_bytes(statement, 1);
    int other_name_size = sqlite3_column_bytes(statement, 2);
    int ctrl_id_size = sqlite3_column_bytes(statement, 4);
    int gate_id_size = sqlite3_column_bytes(statement, 5);

    std::string wire_name_text(name_size, '\0');
    std::vector<uint8_t> other_name_data(other_name_size);
    std::vector<uint8_t> ctrl_id_data(ctrl_id_size);
    std::vector<uint8_t> gate_id_data(gate_id_size);

    std::memcpy(wire_name_text.data(), sqlite3_column_text(statement, 1), name_size);
    std::memcpy(other_name_data.data(), sqlite3_column_blob(statement, 2), other_name_size);
    std::memcpy(ctrl_id_data.data(), sqlite3_column_blob(statement, 4), ctrl_id_size);
    std::memcpy(gate_id_data.data(), sqlite3_column_blob(statement, 5), gate_id_size);

    std::vector<SqlValue> values =
    {
        (int64_t) sqlite3_column_int64(statement, 0),       // id
        wire_name_text,                                     // primary_name
        other_name_data,                                    // other_names
        (int32_t) sqlite3_column_int(statement, 3),         // pulled
        ctrl_id_data,                                       // control_transistors
        gate_id_data                                        // gate_transistors
    };

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
bool ExternalStorage::contains_current(const Wire& object) const
{
    const std::string query = "SELECT * FROM " + _table_name<Wire>() + " WHERE id=?;";
    SqliteStatement statement = _bind_values(query, { _to_sql_type<uint64_t>(object.id())});

    bool contains_current = false;
    int result = sqlite3_step(statement);

    if( result == SQLITE_ROW )
    {
        Wire incoming = _decode<Wire>(statement);

        // Don't forget to set the state of the new wire, since
        // it's used as part of the equality comparison but doesn't
        // matter much here
        incoming.state(object.state());

        contains_current = incoming == object;
    }
    else if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(_db_connection_obj)
        ); 
    }

    return contains_current;
}


template<>
void ExternalStorage::add_component(const Wire& object)
{
    const std::string query = "INSERT INTO " + _table_name<Wire>() + " VALUES " +
        "(?,?,?,?,?,?);";

    SqliteStatement statement = _bind_values(query, _encode(object));
    int result = sqlite3_step(statement);

    if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(_db_connection_obj)
        );
    }
}


template<>
void ExternalStorage::update_component(const Wire& object)
{
    if( !contains(object) )
    {
        add_component(object);
        return;
    }

    const std::string query = "UPDATE " + _table_name<Wire>() + " SET " +
        "primary_name=?002," +
        "other_names=?003," +
        "pulled=?004," +
        "control_transistors=?005," +
        "gate_transistors=?006" +
        " WHERE id=?001;";

    SqliteStatement statement = _bind_values(query, _encode(object));
    int result = sqlite3_step(statement);

    if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(_db_connection_obj)
        );
    }
}


template<>
Wire ExternalStorage::get(const uint64_t id) const
{
    const std::string query = "SELECT * FROM " + _table_name<Wire>() + " WHERE id=?;";
    SqliteStatement statement = _bind_values(query, { _to_sql_type<uint64_t>(id) });

    int result = sqlite3_step(statement);
    if( result == SQLITE_ROW )
    {
        Wire wire = _decode<Wire>(statement);
        return wire;
    }
    else if( result == SQLITE_DONE )
    {
        throw circsim::common::ValueError
        (
            "No wire instance found for id " + std::to_string(id)
        );
    }
    else
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(_db_connection_obj)
        );
    }
}


template<>
std::vector<Wire> ExternalStorage::get_all() const
{
    size_t wire_count = count<Wire>();

    std::vector<Wire> wires;
    wires.reserve(wire_count);

    const std::string query = "SELECT * FROM " + _table_name<Wire>() + ";";
    SqliteStatement statement = _bind_values(query, {});

    int result;
    for(result = sqlite3_step(statement); result == SQLITE_ROW; result = sqlite3_step(statement))
    {
        Wire wire = _decode<Wire>(statement);
        wires.push_back(wire);
    }

    if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(const_cast<sqlite3*>(_db_connection_obj))
        );
    }

    return wires;
}
