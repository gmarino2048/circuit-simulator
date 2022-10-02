/**
 * @file Register.cpp
 * @author Guy Marino (gxm262@case.edu)
 * @brief Contains implementation of Register objects in external storage.
 * @version 0.1
 * @date 2022-10-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <cstring>
#include <string>

// Library Includes
#include <sqlite3.h>

// Project Includes
#include <circsim/common/StateError.hpp>
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Register.hpp>
#include <circsim/data/ExternalStorage.hpp>

using Register = circsim::components::Register;
using namespace circsim::data;


template<>
const std::string ExternalStorage::_table_name<Register>()
{
    return "registers";
}


const size_t REGISTER_FIELD_COUNT = 3;

template<>
void ExternalStorage::_create_table<Register>()
{
    const std::string query = (std::string)
        "CREATE TABLE IF NOT EXISTS " + _table_name<Register>() + " (" +
            "id INTEGER PRIMARY KEY," +
            "name TEXT NOT NULL," +
            "wire_ids BLOB" +
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
std::vector<ExternalStorage::SqlValue> ExternalStorage::_encode(const Register& object) const
{
    std::vector<SqlValue> values;
    values.reserve(REGISTER_FIELD_COUNT);

    values.push_back(_to_sql_type<uint64_t>(object.id()));
    values.push_back(_to_sql_type<std::string>(object.name()));
    values.push_back(_to_sql_type<std::vector<uint64_t>>(object.wire_ids()));

    return values;
}


template<>
Register ExternalStorage::_decode(SqliteStatement& statement) const
{
    int name_size = sqlite3_column_bytes(statement, 1);
    int wire_ids_size = sqlite3_column_bytes(statement, 2);

    std::string register_name_text(name_size, '\0');
    std::vector<uint8_t> wire_ids_data(wire_ids_size);

    std::memcpy(register_name_text.data(), sqlite3_column_text(statement, 1), name_size);
    std::memcpy(wire_ids_data.data(), sqlite3_column_blob(statement, 2), wire_ids_size);

    std::vector<SqlValue> values =
    {
        (int64_t) sqlite3_column_int64(statement, 0),       // id
        register_name_text,                                 // name
        wire_ids_data                                       // wire_ids
    };

    uint64_t id = _from_sql_type<uint64_t>(values[0]);
    std::string name = _from_sql_type<std::string>(values[1]);
    std::vector<uint64_t> wire_ids = _from_sql_type<std::vector<uint64_t>>(values[2]);

    Register reg
    (
        id,
        name,
        wire_ids
    );

    return reg;
}


template<>
void ExternalStorage::add_component(const Register& object)
{
    const std::string query = "INSERT INTO " + _table_name<Register>() + " VALUES " +
        "(?,?,?);";

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
void ExternalStorage::update_component(const Register& object)
{
    if( !contains(object) )
    {
        add_component(object);
        return;
    }

    const std::string query = "UPDATE " + _table_name<Register>() + " SET " +
        "name=?002," +
        "wire_ids=?003" +
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