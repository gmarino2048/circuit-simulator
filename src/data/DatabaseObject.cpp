/**
 * @file DatabaseObject.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Definitions for the DatabaseObject class
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <regex>
#include <sstream>
#include <string>

// Project includes
#include <circsim/common/FormatError.hpp>
#include <circsim/common/ValueError.hpp>

// Declaration file
#include <circsim/data/DatabaseObject.hpp>

using namespace circsim::data;
using DbValue = DatabaseObject::DbValue;

// File constants

const char *DB_NULL_STR = "NULL";
const char *DB_INTEGER_STR = "INTEGER";
const char *DB_REAL_STR = "REAL";
const char *DB_TEXT_STR = "TEXT";
const char *DB_BLOB_STR = "BLOB";


const char *NULL_REGEX = "NULL";

bool DatabaseObject::_validate_null(const std::string &value)
{
    // Make matching as fast as possible
    static const std::regex re_null
    (
        NULL_REGEX,
        std::regex_constants::optimize |
        std::regex_constants::icase
    );

    // Actually perform matching on the string
    std::smatch match;
    bool result = std::regex_match
    (
        value,
        match,
        re_null
    );

    return result;
}


const char *INT_REGEX = "[0-9]+";

bool DatabaseObject::_validate_integer(const std::string &value)
{
    // Make matching as fast as possible
    static const std::regex re_integer
    (
        INT_REGEX,
        std::regex_constants::optimize
    );

    // Match the integer string
    std::smatch match;
    bool result = std::regex_match
    (
        value,
        match,
        re_integer
    );

    return result;
}


const char *REAL_REGEX = "[0-9]+(\\.[0-9])?";

bool DatabaseObject::_validate_real(const std::string &value)
{
    // Make matching as fast as possible
    static const std::regex re_real
    (
        REAL_REGEX,
        std::regex_constants::optimize
    );

    // Match the real string
    std::smatch match;
    bool result = std::regex_match
    (
        value,
        match,
        re_real
    );

    return result;
}


const char *TEXT_REGEX = "\"[A-Za-z0-9_\\-\\[\\]\\(\\)\\?\\!\\., ]*\"";

bool DatabaseObject::_validate_text(const std::string &value)
{
    // Make matching as fast as possible
    static const std::regex re_text
    (
        TEXT_REGEX,
        std::regex_constants::optimize
    );

    // Match the text string
    std::smatch match;
    bool result = std::regex_match
    (
        value,
        match,
        re_text
    );

    return result;
}


void DatabaseObject::_check_values
(
    const DbValue &pkey_value,
    const std::vector<DbColumn> &columns,
    const std::vector<DbValue> &values
)
{
    // First, validate the primary key value
    if ( pkey_value.type != DbType::DBT_INTEGER )
    {
        throw common::FormatError
        (
            "Primary Key Type " +
            type_to_string(pkey_value.type) +
            " Unexpected"
        );
    }
    else if ( !validate_value(pkey_value) )
    {
        throw common::FormatError
        (
            "Primary Key value field \"" +
            pkey_value.value +
            "\" does not conform to expected type " +
            type_to_string(pkey_value.type)
        );
    }

    // Make sure columns and values are the same size
    if ( columns.size() != values.size() )
    {
        throw common::FormatError
        (
            "Column size " +
            std::to_string(columns.size()) +
            " does not match value size " + 
            std::to_string(values.size())
        );
    }

    // Next, check and make sure that the types match
    for ( size_t i = 0; i < columns.size(); i++ )
    {
        if ( columns[i].type != values[i].type )
        {
            throw common::FormatError
            (
                "Column type " +
                type_to_string(columns[i].type) +
                " does not match value type " +
                type_to_string(values[i].type)
            );
        }
    }

    // Finally, check the value type formatting
    for ( const DbValue &value : values )
    {
        if ( !validate_value(value) )
        {
            throw common::FormatError
            (
                "Value field \"" +
                value.value +
                "\" does not conform to expected type " +
                type_to_string(value.type)
            );
        }
    }
}


const char *COLUMN_REGEX = "[A-Za-z][A-Za-z0-9_]*";

bool DatabaseObject::validate_column(const std::string &col_name)
{
    // Make matching as fast as possible
    static const std::regex re_column
    (
        COLUMN_REGEX,
        std::regex_constants::optimize
    );

    // Match the column string
    std::smatch match;
    bool result = std::regex_match
    (
        col_name,
        match,
        re_column
    );

    return result;
}


bool DatabaseObject::validate_value(const DbValue &db_val)
{
    switch ( db_val.type )
    {
        case DbType::DBT_INTEGER:
            return _validate_integer(db_val.value);

        case DbType::DBT_REAL:
            return _validate_real(db_val.value);

        case DbType::DBT_TEXT:
            return _validate_text(db_val.value);

        // NULL and BLOB are unsupported
        default:
            return false;
    }
}


std::string DatabaseObject::type_to_string(const DbType type)
{
    switch ( type )
    {
        case DbType::DBT_NULL:
            return DB_NULL_STR;

        case DbType::DBT_INTEGER:
            return DB_INTEGER_STR;

        case DbType::DBT_REAL:
            return DB_REAL_STR;

        case DbType::DBT_TEXT:
            return DB_TEXT_STR;

        case DbType::DBT_BLOB:
            return DB_BLOB_STR;

        default:
            throw common::ValueError
            (
                "Unrecognized Sqlite3 type: " +
                std::to_string(type)
            );
    }
}


// Format value definitions at end


DatabaseObject::DatabaseObject
(
    const size_t ncols,
    const std::string &table_name,
    const std::string &pkey_column_name
):
    _num_columns(ncols),
    _table_name(table_name),
    _pkey_col_name(pkey_column_name)
{
    // No further content
}


DatabaseObject::~DatabaseObject() = default;


void DatabaseObject::check_columns() const
{
    auto throw_err = [](const std::string &invalid_name)
    {
        throw common::ValueError
        (
            "Invalid database column name: \"" +
            invalid_name + "\""
        );
    };

    if ( !validate_column(_pkey_col_name) )
    {
        throw_err(_pkey_col_name);
    }

    for ( const DbColumn col : _get_columns() )
    {
        if ( !validate_column(col.name) )
        {
            throw_err(col.name);
        }
    }
}


void DatabaseObject::check_values() const
{
    std::vector<DbColumn> columns = _get_columns();

    DbValue pkey_value = _get_primary_key();
    std::vector<DbValue> values = _get_values();

    _check_values
    (
        pkey_value,
        columns,
        values
    );
}


void DatabaseObject::check_values_against_current
(
    const DatabaseObject *other
) const
{
    if ( other == nullptr )
    {
        throw common::ValueError
        (
            "Unexpected null pointer when checking batch values"
        );
    }

    std::vector<DbColumn> columns = this->_get_columns();

    DbValue pkey_value = other->_get_primary_key();
    std::vector<DbValue> values = other->_get_values();

    _check_values
    (
        pkey_value,
        columns,
        values
    );
}


std::string DatabaseObject::create_table() const
{
    // First validate the columns
    check_columns();

    // Use stringstream to construct the table stm
    std::stringstream stream;

    stream << "CREATE TABLE " << _table_name << " ";
    stream << "IF NOT EXISTS (";

    // Add the primary key
    stream << _pkey_col_name << " ";
    stream << "INTEGER PRIMARY KEY NOT NULL";

    // Add the other columns
    for ( const DbColumn &col : _get_columns() )
    {
        stream << ", " << col.name << " ";
        stream << type_to_string(col.type) << " ";

        // All values should be not null for this database
        stream << "NOT NULL";
    }

    stream << ");";
    return stream.str();
}


std::string DatabaseObject::insert_item() const
{
    // Make sure that the columns match the values
    check_columns();
    check_values();

    // Form the insertion string
    std::stringstream stream;
    stream << "INSERT INTO " << _table_name << " ( ";
    stream << _pkey_col_name;

    for ( const DbColumn &col : _get_columns() )
    {
        stream << ", ";
        stream << col.name;
    }

    stream << " ) VALUES ( ";
    stream << _get_primary_key().value;

    for ( const DbValue &val : _get_values() )
    {
        stream << ", ";
        stream << val.value;
    }

    stream << " );";
    return stream.str();
}


std::string DatabaseObject::insert_all
(
    const std::vector<DatabaseObject*> &others
) const
{
    // Make sure the columns are good to go
    check_columns();

    // Form the insertion string
    std::stringstream stream;
    stream << "INSERT INTO " << _table_name << " ( ";
    stream << _pkey_col_name;

    for ( const DbColumn &col : _get_columns() )
    {
        stream << ", ";
        stream << col.name;
    }

    stream << " ) VALUES ";

    using DbIterator = std::vector<DatabaseObject*>::const_iterator;
    for ( DbIterator it = others.begin(); it != others.end(); it++ )
    {
        if ( it != others.begin() )
        {
            stream << ", ";
        }

        check_values_against_current(*it);
        stream << "( " << (*it)->_get_primary_key().value;

        for ( const DbValue &val : (*it)->_get_values() )
        {
            stream << ", ";
            stream << val.value;
        }

        stream << " )";
    }

    stream << ";";
    return stream.str();
}


template<>
DbValue DatabaseObject::format_value<size_t>(const size_t &object)
{
    std::string value_string = std::to_string(object);
    DbValue value =
    {
        .type = DbType::DBT_INTEGER,
        .value = value_string
    };

    return value;
}


template<>
DbValue DatabaseObject::format_value<bool>(const bool &object)
{
    size_t int_value = object ? 1 : 0;
    return format_value<size_t>(int_value);
}


const char *STRING_REGEX = "[A-Za-z0-9_\\-\\[\\]\\(\\)\\?\\!\\. ]*";

template<>
DbValue DatabaseObject::format_value<std::string>(const std::string &object)
{
    // Check the string value
    static const std::regex string_match
    (
        STRING_REGEX,
        std::regex_constants::optimize
    );

    std::smatch match;
    bool valid = std::regex_match(object, match, string_match);

    if ( valid )
    {
        std::string value_string = "\"" + object + "\"";
        DbValue value =
        {
            .type = DbType::DBT_TEXT,
            .value = value_string
        };

        return value;
    }
    else
    {
        throw common::ValueError
        (
            "Invalid characters found in string \"" +
            object + "\""
        );
    }
}


using IntList = std::vector<size_t>;

template<>
DbValue DatabaseObject::format_value<IntList>(const IntList &object)
{
    std::stringstream stream;
    stream << "\"";

    IntList::const_iterator it = object.begin();
    stream << std::to_string(*it);
    it++;

    while ( it != object.end() )
    {
        stream << ",";
        stream << std::to_string(*it);

        it++;
    }

    stream << "\"";

    DbValue value =
    {
        .type = DbType::DBT_TEXT,
        .value = stream.str()
    };

    return value;
}


using StringList = std::vector<std::string>;

template<>
DbValue DatabaseObject::format_value<StringList>(const StringList &object)
{
    static const std::regex string_match
    (
        STRING_REGEX,
        std::regex_constants::optimize
    );

    std::stringstream stream;
    stream << "\"";

    for
    (
        StringList::const_iterator it = object.begin();
        it != object.end();
        it++
    )
    {
        if ( it != object.begin() )
        {
            stream << ",";
        }

        std::smatch match;
        bool re_match = std::regex_match
        (
            *it,
            match,
            string_match
        );

        if ( !re_match )
        {
            throw common::ValueError
            (
                "Invalid characters found in string \"" +
                *it + "\""
            );
        }

        stream << "'" << *it << "'";
    }

    stream << "\"";
    DbValue value =
    {
        .type = DbType::DBT_TEXT,
        .value = stream.str()
    };

    return value;
}
