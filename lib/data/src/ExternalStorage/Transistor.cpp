
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
        throw circsim::common::StateError(sqlite3_errstr(result));
    }

    sqlite3_finalize(statement);
}

