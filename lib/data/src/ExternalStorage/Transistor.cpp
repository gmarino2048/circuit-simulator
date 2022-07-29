
#include <sqlite3.h>

#include <circsim/common/StateError.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/data/ExternalStorage.hpp>

using Transistor = circsim::components::Transistor;
using namespace circsim::data;

// Template Instantiation of ExternalStorage::_table_name
template<>
const std::string ExternalStorage::_table_name<Transistor>();

// Template Instantiation of ExternalStorage::_create_platform
template<>
void ExternalStorage::_create_table<Transistor>();


template<>
const std::string ExternalStorage::_table_name<Transistor>()
{
    return "transistors";
}


template<>
void ExternalStorage::_create_table<Transistor>()
{
    const std::string query = (std::string)
        "CREATE TABLE IF NOT EXISTS " + _table_name<Transistor>() + "(" +
            "id INTEGER PRIMARY_KEY," +
            "name TEXT NOT NULL," +
            "type INTEGER NOT NULL," +
            "gate INTEGER NOT NULL," +
            "source INTEGER NOT NULL," +
            "drain INTEGER NOT NULL," +
        ");";

    sqlite3_stmt* statement = _bind_values(query, {});

    int result = sqlite3_step(statement);
    if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError(sqlite3_errmsg(_db_connection_obj));
    }

    sqlite3_finalize(statement);
}


template<>
bool ExternalStorage::contains<Transistor>(const Transistor& object) const
{
    uint64_t id = object.id();

    const std::string query = "SELECT * FROM " + _table_name<Transistor>() + " WHERE id=?;";
    sqlite3_stmt* statement = _bind_values(query, { _to_sql_type(id) });

    bool contains = false;
    int result = sqlite3_step(statement);

    if( result == SQLITE_ROW )
    {
        contains = true;
    }
    else if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(const_cast<sqlite3*>(_db_connection_obj))
        );
    }

    sqlite3_finalize(statement);
    return contains;
}
