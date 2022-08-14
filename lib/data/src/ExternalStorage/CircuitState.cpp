/**
 * @file CircuitState.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Implementation for storing the circuit state object within the external
 *        storage object.
 * @version 0.1
 * @date 2022-08-13
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
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>
#include <circsim/components/CircuitState.hpp>
#include <circsim/data/ExternalStorage.hpp>

using namespace circsim::components;
using namespace circsim::data;


template<>
const std::string ExternalStorage::_table_name<CircuitState>()
{
    return "snapshots";
}


const size_t CIRCUIT_STATE_FIELD_COUNT = 5;

template<>
void ExternalStorage::_create_table<CircuitState>()
{
    const std::string query = "CREATE TABLE IF NOT EXISTS " + _table_name<CircuitState>() + " (" +
        "id INTEGER PRIMARY KEY," +
        "transistor_ids BLOB," +
        "transistor_states BLOB," +
        "wire_ids BLOB," +
        "wire_states BLOB" +
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
std::vector<ExternalStorage::SqlValue> ExternalStorage::_encode(const CircuitState& object) const
{
    std::vector<SqlValue> values;
    values.reserve(CIRCUIT_STATE_FIELD_COUNT);

    std::vector<CircuitState::ObjectState<Transistor>> transistor_state_list = object.get_all_states<Transistor>();
    std::vector<CircuitState::ObjectState<Wire>> wire_state_list = object.get_all_states<Wire>();

    std::vector<uint64_t> transistor_ids;
    std::vector<uint8_t> transistor_states;

    transistor_ids.reserve(transistor_state_list.size());
    transistor_states.reserve(transistor_state_list.size());
    for( const auto [id, state] : transistor_state_list )
    {
        transistor_ids.push_back(id);
        transistor_states.push_back(state);
    }

    std::vector<uint64_t> wire_ids;
    std::vector<uint8_t> wire_states;

    wire_ids.reserve(wire_state_list.size());
    wire_states.reserve(wire_state_list.size());
    for( const auto [id, state] : wire_state_list )
    {
        wire_ids.push_back(id);
        wire_states.push_back(state);
    }

    values.push_back(_to_sql_type<uint64_t>(object.id()));
    values.push_back(_to_sql_type<std::vector<uint64_t>>(transistor_ids));
    values.push_back(_to_sql_type<std::vector<uint8_t>>(transistor_states));
    values.push_back(_to_sql_type<std::vector<uint64_t>>(wire_ids));
    values.push_back(_to_sql_type<std::vector<uint8_t>>(wire_states));

    return values;
}


template<>
CircuitState ExternalStorage::_decode(SqliteStatement& statement) const
{
    int transistor_id_size = sqlite3_column_bytes(statement, 1);
    int transistor_state_size = sqlite3_column_bytes(statement, 2);
    int wire_id_size = sqlite3_column_bytes(statement, 3);
    int wire_state_size = sqlite3_column_bytes(statement, 4);

    std::vector<uint8_t>
        transistor_id_data(transistor_id_size),
        transistor_state_data(transistor_state_size),
        wire_id_data(wire_id_size),
        wire_state_data(wire_state_size);

    std::memcpy(transistor_id_data.data(), sqlite3_column_blob(statement, 1), transistor_id_size);
    std::memcpy(transistor_state_data.data(), sqlite3_column_blob(statement, 2), transistor_state_size);
    std::memcpy(wire_id_data.data(), sqlite3_column_blob(statement, 3), wire_id_size);
    std::memcpy(wire_state_data.data(), sqlite3_column_blob(statement, 4), wire_state_size);

    std::vector<SqlValue> values =
    {
        (int64_t) sqlite3_column_int64(statement, 0),           // id
        transistor_id_data,                                     // transistor_ids
        transistor_state_data,                                  // transistor_states
        wire_id_data,                                           // wire_ids
        wire_state_data,                                        // wire_states
    };

    uint64_t id = _from_sql_type<uint64_t>(values[0]);
    std::vector<uint64_t> transistor_ids = _from_sql_type<std::vector<uint64_t>>(values[1]);
    std::vector<uint64_t> wire_ids = _from_sql_type<std::vector<uint64_t>>(values[3]);

    std::vector<uint8_t> transistor_temp = _from_sql_type<std::vector<uint8_t>>(values[2]);
    std::vector<uint8_t> wire_temp = _from_sql_type<std::vector<uint8_t>>(values[4]);

    std::vector<Transistor::State> transistor_states;
    std::transform
    (
        transistor_temp.begin(),
        transistor_temp.end(),
        std::back_inserter(transistor_states),
        [](const uint8_t value)
        {
            return static_cast<Transistor::State>(value);
        }
    );

    std::vector<Wire::State> wire_states;
    std::transform
    (
        wire_temp.begin(),
        wire_temp.end(),
        std::back_inserter(wire_states),
        [](const uint8_t value)
        {
            return static_cast<Wire::State>(value);
        }
    );

    CircuitState state_object(id);
    state_object.update_all_states<Transistor>(transistor_ids, transistor_states);
    state_object.update_all_states<Wire>(wire_ids, wire_states);

    return state_object;
}


template<>
void ExternalStorage::add_component(const CircuitState& object)
{
    const std::string query = "INSERT INTO " + _table_name<CircuitState>() + " VALUES " +
        "(?,?,?,?,?)";

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
void ExternalStorage::update_component(const CircuitState& object)
{
    if( !contains(object) )
    {
        add_component(object);
        return;
    }

    const std::string query = "UPDATE " + _table_name<CircuitState>() + " SET " +
        "transistor_ids=?002," +
        "transistor_states=?003," +
        "wire_ids=?004," +
        "wire_states=?005" +
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
