
#include <vector>
#include <sstream>

#include <sqlite3.h>

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
            "id INTEGER PRIMARY_KEY," +
            "name TEXT NOT NULL," +
            "type INTEGER NOT NULL," +
            "gate INTEGER NOT NULL," +
            "source INTEGER NOT NULL," +
            "drain INTEGER NOT NULL"
        ");";

    sqlite3_stmt* statement = _bind_values(query, {});

    int result = sqlite3_step(statement);
    if( result != SQLITE_DONE )
    {
        throw circsim::common::StateError(sqlite3_errmsg(_db_connection_obj));
    }

    sqlite3_finalize(statement);
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
Transistor ExternalStorage::_decode(const std::vector<SqlValue>& values) const
{
    using TransType = circsim::components::Transistor::Type;
    using TransState = circsim::components::Transistor::State;

    if( values.size() != TRANSISTOR_FIELD_COUNT )
    {
        std::stringstream ss;
        ss << "Cannot convert to transistor. Expected " << std::to_string(TRANSISTOR_FIELD_COUNT);
        ss << " fields but got " << std::to_string(values.size()) << ".";

        throw circsim::common::ValueError
        (
            ss.str()
        );
    }

    uint64_t id = _from_sql_type<uint64_t>(values[0]);
    std::string name = _from_sql_type<std::string>(values[1]);
    TransType type = static_cast<TransType>(_from_sql_type<uint8_t>(values[2]));

    uint64_t gate = _from_sql_type<uint64_t>(values[3]);
    uint64_t source = _from_sql_type<uint64_t>(values[4]);
    uint64_t drain = _from_sql_type<uint64_t>(values[5]);

    TransState state = static_cast<TransState>(_from_sql_type<uint8_t>(values[6]));

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
        sqlite3_finalize(statement);
        throw circsim::common::StateError
        (
            sqlite3_errmsg(const_cast<sqlite3*>(_db_connection_obj))
        );
    }

    sqlite3_finalize(statement);
    return contains;
}


template<>
bool ExternalStorage::contains_current<Transistor>(const Transistor& object) const
{
    uint64_t id = object.id();

    const std::string query = "SELECT * FROM " + _table_name<Transistor>() + " WHERE id=?;";
    sqlite3_stmt* statement = _bind_values(query, { _to_sql_type(id) });

    bool contains_current = false;
    int result = sqlite3_step(statement);

    if( result == SQLITE_ROW )
    {
        std::vector<SqlValue> incoming =
        {
            (int64_t) sqlite3_column_int64(statement, 0),       // id
            (const char *) sqlite3_column_text(statement, 1),   // name
            (int32_t) sqlite3_column_int(statement, 2),         // type
            (int64_t) sqlite3_column_int64(statement, 3),       // gate
            (int64_t) sqlite3_column_int64(statement, 4),       // source
            (int64_t) sqlite3_column_int64(statement, 5)        // drain
        };

        Transistor compare_to = _decode<Transistor>(incoming);
        contains_current = object == compare_to;
    }
    else if( result != SQLITE_DONE )
    {
        sqlite3_finalize(statement);
        throw circsim::common::StateError
        (
            sqlite3_errmsg(const_cast<sqlite3*>(_db_connection_obj))
        );
    }

    sqlite3_finalize(statement);
    return contains_current;
}
