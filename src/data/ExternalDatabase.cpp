/**
 * @file ExternalDatabase.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief The object definitions for the External Database of the simulator.
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <string>
#include <optional>
#include <vector>

// Library Includes
#include <sqlite3.h>

// Project Includes
#include <circsim/common/StateError.hpp>
#include <circsim/common/ValueError.hpp>
#include <circsim/data/ExternalDatabase.hpp>
#include <circsim/data/objects/WireDatabaseObject.hpp>
#include <circsim/data/objects/TransistorDatabaseObject.hpp>

using namespace circsim::common;
using namespace circsim::data;
using namespace circsim::data::objects;

void ExternalDatabase::_prepare_statement
(
    const std::string &query,
    sqlite3_stmt *statement
)
{
    const char *c_query = query.c_str();
    const char *query_end = &(*query.end());

    const char *statement_parse_end = nullptr;
    int preparation_result = ::sqlite3_prepare_v2
    (
        this->_db_connection_obj,
        c_query,
        query.size() * sizeof(std::string::value_type),
        &statement,
        &statement_parse_end
    );

    if ( statement_parse_end != query_end )
    {
        throw StateError
        (
            "Query \"" + query + "\" " +
            "was not completely consumed during preparation. " +
            "Formatting error expected."
        );
    }

    if ( preparation_result != SQLITE_OK )
    {
        const char *errmsg = ::sqlite3_errmsg(this->_db_connection_obj);
        throw StateError
        (
            (std::string) "Database Error: " + errmsg
        );
    }
}

template<>
std::vector<std::nullopt_t> ExternalDatabase::_fetch_results(sqlite3_stmt *statement)
{
    int step_result = ::sqlite3_step(statement);

    if ( step_result == SQLITE_ROW )
    {
        throw ValueError
        (
            (std::string)
            "A return value was not expected from this query, " +
            "but one was returned anyway."
        );
    }

    if ( step_result == SQLITE_DONE )
    {
        return {};
    }
    else
    {
        const char *errmsg = ::sqlite3_errmsg(this->_db_connection_obj);
        throw StateError
        (
            (std::string)
            "Database Error: " + errmsg
        );
    }
}


template<class DatabaseObjectT>
std::vector<DatabaseObjectT> ExternalDatabase::_fetch_results(sqlite3_stmt *statement)
{
    std::vector<DatabaseObjectT> objects;

    int step_result;
    for
    (
        step_result = ::sqlite3_step(statement);
        step_result == SQLITE_ROW;
        // Row is automatically iterated
    )
    {
        DatabaseObjectT &obj_ref = objects.emplace_back();

        // Enforce direct inheritence from DatabaseObject
        DatabaseObject *obj_pointer = static_cast<DatabaseObject*>(&obj_ref);
        obj_pointer->import(statement);
    }

    if ( step_result != SQLITE_DONE )
    {
        const char *errmsg = ::sqlite3_errmsg(this->_db_connection_obj);
        throw StateError
        (
            (std::string)
            "Database Error: " + errmsg
        );
    }

    return objects;
}

template std::vector<WireDatabaseObject>
ExternalDatabase::_fetch_results<WireDatabaseObject>(sqlite3_stmt *statement);

template std::vector<TransistorDatabaseObject>
ExternalDatabase::_fetch_results<TransistorDatabaseObject>(sqlite3_stmt *statement);


bool ExternalDatabase::_table_exists(const std::string &table_name)
{
    // TODO: fill this in
    return true;
}
