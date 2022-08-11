/**
 * @file Transistor.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Contains implementation of Transistor objects in external storage.
 * @version 0.1
 * @date 2022-08-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <cstring>
#include <vector>
#include <sstream>

// Library Includes
#include <sqlite3.h>

// Project Includes
#include <circsim/common/StateError.hpp>
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/data/ExternalStorage.hpp>

using Transistor = circsim::components::Transistor;
using namespace circsim::data;


template<>
const std::string ExternalStorage::_table_name<Transistor>()
{
    return "transistors";
}


template<>
void ExternalStorage::_create_table<Transistor>()
{
    const std::string query = (std::string)
        "CREATE TABLE IF NOT EXISTS " + _table_name<Transistor>() + " (" +
            "id INTEGER PRIMARY KEY," +
            "name TEXT NOT NULL," +
            "type INTEGER NOT NULL," +
            "gate INTEGER NOT NULL," +
            "source INTEGER NOT NULL," +
            "drain INTEGER NOT NULL"
        ");";

    SqliteStatement statement = _bind_values(query, {});

    int result = sqlite3_step(statement);
    if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError(sqlite3_errmsg(_db_connection_obj));
    }
}


static const size_t TRANSISTOR_FIELD_COUNT = 6;

template<>
std::vector<ExternalStorage::SqlValue> ExternalStorage::_encode(const Transistor& object) const
{
    std::vector<SqlValue> values(TRANSISTOR_FIELD_COUNT);

    values[0] = _to_sql_type<uint64_t>(object.id());
    values[1] = _to_sql_type<std::string>(object.name());
    values[2] = _to_sql_type<uint8_t>(object.type());
    values[3] = _to_sql_type<uint64_t>(object.gate());
    values[4] = _to_sql_type<uint64_t>(object.source());
    values[5] = _to_sql_type<uint64_t>(object.drain());

    return values;
}


template<>
Transistor ExternalStorage::_decode(SqliteStatement& statement) const
{
    using TransType = circsim::components::Transistor::Type;

    int name_size = sqlite3_column_bytes(statement, 1);
    std::string transistor_name(name_size, '\0');

    const unsigned char* sql_name = sqlite3_column_text(statement, 1);
    std::memcpy(transistor_name.data(), sql_name, name_size);

    std::vector<SqlValue> values =
    {
        (int64_t) sqlite3_column_int64(statement, 0),       // id
        transistor_name,                                    // name
        (int32_t) sqlite3_column_int(statement, 2),         // type
        (int64_t) sqlite3_column_int64(statement, 3),       // gate
        (int64_t) sqlite3_column_int64(statement, 4),       // source
        (int64_t) sqlite3_column_int64(statement, 5)        // drain
    };

    uint64_t id = _from_sql_type<uint64_t>(values[0]);
    std::string name = _from_sql_type<std::string>(values[1]);
    TransType type = static_cast<TransType>(_from_sql_type<uint8_t>(values[2]));

    uint64_t gate = _from_sql_type<uint64_t>(values[3]);
    uint64_t source = _from_sql_type<uint64_t>(values[4]);
    uint64_t drain = _from_sql_type<uint64_t>(values[5]);

    circsim::components::Transistor transistor
    (
        id,
        name,
        gate,
        source,
        drain,
        type
    );

    return transistor;
}


template<>
bool ExternalStorage::contains_current<Transistor>(const Transistor& object) const
{
    uint64_t id = object.id();

    const std::string query = "SELECT * FROM " + _table_name<Transistor>() + " WHERE id=?;";
    SqliteStatement statement = _bind_values(query, { _to_sql_type(id) });

    bool contains_current = false;
    int result = sqlite3_step(statement);

    if( result == SQLITE_ROW )
    {
        Transistor compare_to = _decode<Transistor>(statement);
        contains_current = object == compare_to;
    }
    else if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(const_cast<sqlite3*>(_db_connection_obj))
        );
    }

    return contains_current;
}


template<>
void ExternalStorage::add_component(const Transistor& object)
{
    const std::string query = "INSERT INTO " + _table_name<Transistor>() +" VALUES (?,?,?,?,?,?);";

    SqliteStatement statement = _bind_values(query, _encode(object));
    int result = sqlite3_step(statement);

    if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(const_cast<sqlite3*>(_db_connection_obj))
        );
    }
}


template<>
void ExternalStorage::update_component(const Transistor& object)
{
    if( !contains(object) )
    {
        add_component(object);
        return;
    }

    const std::string query = "UPDATE " + _table_name<Transistor>() + " SET " +
        "name=?002," +
        "type=?003," +
        "gate=?004," +
        "source=?005," +
        "drain=?006" +
        " WHERE id=?001;";

    SqliteStatement statement = _bind_values(query, _encode(object));
    int result = sqlite3_step(statement);

    if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(const_cast<sqlite3*>(_db_connection_obj))
        );
    }
}


template<>
Transistor ExternalStorage::get(const uint64_t id) const
{
    Transistor value;
    const std::string query = "SELECT * FROM " + _table_name<Transistor>() + " WHERE id=?;";

    SqliteStatement statement = _bind_values(query, { _to_sql_type<uint64_t>(id) });
    int result = sqlite3_step(statement);

    if( result == SQLITE_ROW )
    {
        value = _decode<Transistor>(statement);
    }
    else
    {
        throw circsim::common::StateError
        (
            "No value found with ID " + std::to_string(id)
        );
    }

    result = sqlite3_step(statement);
    if( result != SQLITE_DONE )
    {
        if(result == SQLITE_ROW )
        {
            throw circsim::common::StateError
            (
                "Multiple values found with same ID. This should not be possible."
            );
        }
        else
        {
            throw circsim::common::StateError
            (
                sqlite3_errmsg(const_cast<sqlite3*>(_db_connection_obj))
            );
        }
    }

    return value;
}


template<>
std::vector<Transistor> ExternalStorage::get_all() const
{
    size_t item_count = count<Transistor>();

    std::vector<Transistor> transistor_list;
    transistor_list.reserve(item_count);

    const std::string query = "SELECT * FROM " + _table_name<Transistor>() + ";";
    SqliteStatement statement = _bind_values(query, {});

    int result = 0;
    for(result = sqlite3_step(statement); result == SQLITE_ROW; result = sqlite3_step(statement))
    {
        Transistor value = _decode<Transistor>(statement);
        transistor_list.push_back(value);
    }

    if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(const_cast<sqlite3*>(_db_connection_obj))
        );
    }

    return transistor_list;
}
