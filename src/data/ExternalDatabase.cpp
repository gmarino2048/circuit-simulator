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
