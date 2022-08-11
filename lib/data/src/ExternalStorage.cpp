/**
 * @file ExternalStorage.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Add common definitions for the ExternalStorage module. Note
 *        that this file only contains the common definitions and not
 *        object specific ones, which can be found in the ExternalStorage
 *        directory.
 * @version 0.1
 * @date 2022-07-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <algorithm>
#include <cstring>
#include <limits>

// Library includes
#include <sqlite3.h>

// Project Includes
#include <circsim/common/EndianOperations.hpp>
#include <circsim/common/IndexError.hpp>
#include <circsim/common/StateError.hpp>
#include <circsim/common/ValueError.hpp>

#include <circsim/components/Transistor.hpp>

#include <circsim/data/ExternalStorage.hpp>

using namespace circsim::data;
using SqlValue = ExternalStorage::SqlValue;

// File constants
const char NULL_TERMINATOR = '\0';

// Template Initialization of ExternalStorage::_to_sql_type
template<>
SqlValue ExternalStorage::_to_sql_type(const uint8_t value);

template<>
SqlValue ExternalStorage::_to_sql_type(const uint64_t value);

template<>
SqlValue ExternalStorage::_to_sql_type(const std::vector<uint64_t> value);

template<>
SqlValue ExternalStorage::_to_sql_type(const std::string value);

template<>
SqlValue ExternalStorage::_to_sql_type(const std::vector<std::string> value);


// Template Initialization of ExternalStorage::_from_sql_type
template<>
uint8_t ExternalStorage::_from_sql_type(const SqlValue& value);

template<>
uint64_t ExternalStorage::_from_sql_type(const SqlValue& value);

template<>
std::vector<uint64_t> ExternalStorage::_from_sql_type(const SqlValue& value);

template<>
std::string ExternalStorage::_from_sql_type(const SqlValue& value);

template<>
std::vector<std::string> ExternalStorage::_from_sql_type(const SqlValue& value);


// Implementation of SqliteStatement wrapper
ExternalStorage::SqliteStatement::SqliteStatement(sqlite3_stmt* statement) : _statement(statement) {}


ExternalStorage::SqliteStatement::SqliteStatement(SqliteStatement&& other) noexcept :
    _statement(other._statement)
{
    other._statement = nullptr;
}


ExternalStorage::SqliteStatement::~SqliteStatement()
{
    if( _statement != nullptr )
    {
        sqlite3_finalize(_statement);
        _statement = nullptr;
    }
}


ExternalStorage::SqliteStatement& ExternalStorage::SqliteStatement::operator=
(
    SqliteStatement&& other
) noexcept
{
    this->_statement = other._statement;
    other._statement = nullptr;

    return *this;
}


ExternalStorage::SqliteStatement::operator sqlite3_stmt*()
{
    return _statement;
}


// Implementation of ExternalStorage::_to_sql_type
template<>
SqlValue ExternalStorage::_to_sql_type(const uint8_t value)
{
    // Simple cast is fine since size goes up
    return SqlValue(static_cast<int32_t>(value));
}

template<>
SqlValue ExternalStorage::_to_sql_type(const uint64_t value)
{
    // Do a bitwise conversion to avoid overflow issues
    const uint64_t* value_ptr = &value;
    int64_t converted_value = *(reinterpret_cast<const int64_t*>(value_ptr));

    return SqlValue(converted_value);
}

template<>
SqlValue ExternalStorage::_to_sql_type(const std::vector<uint64_t> value)
{
    // Convert the internal vector buffer to a void pointer
    std::vector<uint64_t> be_values;
    be_values.reserve(value.size());

    std::transform
    (
        value.begin(),
        value.end(),
        std::back_inserter(be_values),
        []( const uint64_t number_value )
        {
            return circsim::common::EndianOperations::host_to_big_endian(number_value);
        }
    );

    uint64_t byte_count = sizeof(uint64_t) * be_values.size();
    std::vector<uint8_t> bytes_vector(byte_count);

    std::memcpy
    (
        (void*)bytes_vector.data(),
        (void*)be_values.data(),
        byte_count
    );

    return SqlValue(bytes_vector);
}

template<>
SqlValue ExternalStorage::_to_sql_type(const std::string value)
{
    // No need to do type conversion
    return SqlValue(value);
}

template<>
SqlValue ExternalStorage::_to_sql_type(const std::vector<std::string> value)
{
    size_t byte_count = 0;
    for(const std::string& str : value)
    {
        // Add the amount of space needed for each string
        // Plus one for the null terminator value
        byte_count += str.size() + 1;
    }

    // Copy the values to the byte vector object
    std::vector<uint8_t> byte_array;
    byte_array.reserve(byte_count);

    for(const std::string& str : value)
    {
        std::copy
        (
            str.begin(),
            str.end(),
            std::back_inserter(byte_array)
        );
        byte_array.push_back(NULL_TERMINATOR);
    }

    return SqlValue(byte_array);
}


// Implementation of ExternalStorage::_from_sql_type
template<>
uint8_t ExternalStorage::_from_sql_type(const SqlValue& value) try
{
    int32_t num_value = std::get<int32_t>(value);
    if( num_value < 0 || num_value > std::numeric_limits<uint8_t>::max() )
    {
        throw circsim::common::ValueError
        (
            "SQL Value \"" + std::to_string(num_value) + "\" is not within uint8_t bounds."
        );
    }
    else
    {
        return static_cast<uint8_t>(num_value);
    }
}
catch( const std::bad_variant_access& )
{
    throw circsim::common::IndexError
    (
        "SQL type does not contain int32, required for conversion to uint8."
    );
}

template<>
uint64_t ExternalStorage::_from_sql_type(const SqlValue& value) try
{
    const int64_t num_value = std::get<int64_t>(value);

    // Direct pointer-cast to uint64_t so we don't lose resolution
    uint64_t converted_value = *(reinterpret_cast<const uint64_t*>(&num_value));
    return converted_value;
}
catch( const std::bad_variant_access& )
{
    throw circsim::common::IndexError
    (
        "SQL type does not contain int64, required for conversion to size."
    );
}

template<>
std::vector<uint64_t> ExternalStorage::_from_sql_type(const SqlValue& value) try
{
    const std::vector<uint8_t>& buffer = std::get<std::vector<uint8_t>>(value);
    uint64_t buffer_byte_count = buffer.size();

    if( buffer_byte_count % sizeof(uint64_t) != 0 )
    {
        throw circsim::common::ValueError
        (
            "Size of buffer (" + std::to_string(buffer_byte_count) + 
            ") does not align with size of uint64_t (" + std::to_string(sizeof(uint64_t)) + ")."
        );
    }

    size_t value_count = buffer_byte_count / sizeof(uint64_t);
    std::vector<uint64_t> values(value_count);

    std::memcpy
    (
        (void*)values.data(),
        (void*)buffer.data(),
        buffer_byte_count
    );

    std::transform
    (
        values.begin(),
        values.end(),
        values.begin(),
        []( const uint64_t value )
        {
            return circsim::common::EndianOperations::big_endian_to_host(value);
        }
    );

    return values;
}
catch( const std::bad_variant_access& )
{
    throw circsim::common::IndexError
    (
        "SQL type does not contain blob, required for conversion to vector<size>."
    );
}

template<>
std::string ExternalStorage::_from_sql_type(const SqlValue& value) try
{
    return std::get<std::string>(value);
}
catch( const std::bad_variant_access& )
{
    throw circsim::common::IndexError
    (
        "SQL type does not contain string, required for conversion to string."
    );
}

template<>
std::vector<std::string> ExternalStorage::_from_sql_type(const SqlValue& value) try
{
    const std::vector<uint8_t>& buffer = std::get<std::vector<uint8_t>>(value);
    std::vector<std::string> values;

    std::vector<uint8_t>::const_iterator string_start, string_end;
    string_start = buffer.begin();
    string_end = string_start;

    while( string_end != buffer.end() )
    {
        if( *string_end == NULL_TERMINATOR )
        {
            size_t string_char_count = std::distance(string_start, string_end);

            std::string value(string_char_count, NULL_TERMINATOR);
            std::memcpy
            (
                (void*) value.data(),
                (void*) &(*string_start),
                string_char_count * sizeof(std::string::value_type)
            );

            values.push_back(value);

            // Update the pointer to the beginning character of the next string
            string_start = ++string_end;
        }
        else
        {
            string_end++;
        }
    }

    return values;
}
catch( const std::bad_variant_access& )
{
    throw circsim::common::IndexError
    (
        "SQL type does not contain blob, required for conversion to vector<string>."
    );
}


ExternalStorage::SqliteStatement ExternalStorage::_bind_values
(
    const std::string& query,
    const std::vector<SqlValue>& values
) const
{
    sqlite3_stmt* statement = nullptr;
    int result = sqlite3_prepare_v2
    (
        const_cast<sqlite3*>(_db_connection_obj),
        query.c_str(),
        query.size(),
        &statement,
        nullptr
    );

    if( result != SQLITE_OK )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(_db_connection_obj)
        );
    }

    for( int i = 0; i < values.size() && i < std::numeric_limits<int>::max(); i++ )
    {
        int idx = i+1;
        int result = SQLITE_OK;
        SqlValue value = values[i];

        if( std::holds_alternative<int32_t>(value) )
        {
            result = sqlite3_bind_int(statement, idx, std::get<int32_t>(value));
        }
        else if( std::holds_alternative<int64_t>(value) )
        {
            result = sqlite3_bind_int64(statement, idx, std::get<int64_t>(value));
        }
        else if( std::holds_alternative<double>(value) )
        {
            result = sqlite3_bind_double(statement, idx, std::get<double>(value));
        }
        else if( std::holds_alternative<std::string>(value) )
        {
            std::string value_str = std::get<std::string>(value);
            result = sqlite3_bind_text
            (
                statement,
                idx,
                value_str.c_str(),
                value_str.size() * sizeof(std::string::value_type),
                SQLITE_TRANSIENT
            );
        }
        else if( std::holds_alternative<std::vector<uint8_t>>(value) )
        {
            std::vector<uint8_t> value_data = std::get<std::vector<uint8_t>>(value);
            result = sqlite3_bind_blob
            (
                statement,
                idx,
                reinterpret_cast<const void*>(value_data.data()),
                value_data.size(),
                SQLITE_TRANSIENT
            );
        }
        else
        {
            result = sqlite3_bind_null(statement, idx);
        }

        if( result != SQLITE_OK )
        {
            throw circsim::common::StateError(sqlite3_errstr(result));
        }
    }

    return statement;
}


// Explicit instantiation of objects
template bool ExternalStorage::_table_exists<circsim::components::Transistor>();
template bool ExternalStorage::_table_exists<circsim::components::Wire>();

template<class T>
bool ExternalStorage::_table_exists()
{
    // Compile the query
    const std::string query =
        "SELECT count(type) FROM sqlite_master WHERE type='table' AND name=?;";

    SqliteStatement statement = _bind_values(query, { _table_name<T>() });

    // Run the query
    int table_count = 0;
    for( int result = sqlite3_step(statement); result != SQLITE_DONE; result = sqlite3_step(statement) )
    {
        if( result == SQLITE_ROW )
        {
            table_count = sqlite3_column_int(statement, 0);
            break;
        }
        else
        {
            throw circsim::common::StateError(sqlite3_errstr(result));
        }
    }

    return table_count > 0;
}


void ExternalStorage::_create_tables()
{
    _create_table<circsim::components::Transistor>();
    _create_table<circsim::components::Wire>();
}


// Handle constructor and destructor
ExternalStorage::ExternalStorage()
{
    int flags =
        SQLITE_OPEN_READWRITE |
        SQLITE_OPEN_MEMORY |
        SQLITE_OPEN_NOMUTEX |
        SQLITE_OPEN_PRIVATECACHE;

    int result = sqlite3_open_v2
    (
        ":memory:", // This is overkill, but it doesn't matter
        &_db_connection_obj,
        flags,
        NULL        // Use default VFS
    );

    if( result != SQLITE_OK )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(_db_connection_obj)
        );
    }

    _create_tables();
}


ExternalStorage::ExternalStorage(const std::filesystem::path& db_path)
{
    int flags =
        SQLITE_OPEN_READWRITE |
        SQLITE_OPEN_NOMUTEX;

    int result = sqlite3_open_v2
    (
        db_path.c_str(),
        &_db_connection_obj,
        flags,
        NULL        // Use default VFS
    );

    if( result != SQLITE_OK )
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(_db_connection_obj)
        );
    }

    _create_tables();
}


ExternalStorage::~ExternalStorage()
{
    if( _db_connection_obj != nullptr )
    {
        int result = SQLITE_BUSY;
        while( result == SQLITE_BUSY )
        {
            result = sqlite3_close_v2(_db_connection_obj);
        }
    }
}


// Explicitly initialize count for transistor
template size_t ExternalStorage::count<circsim::components::Transistor>() const;
template size_t ExternalStorage::count<circsim::components::Wire>() const;

template<class T>
size_t ExternalStorage::count() const
{
    std::string query = "SELECT COUNT(*) FROM " + _table_name<T>() + ";";

    SqliteStatement statement = const_cast<const ExternalStorage*>(this)->_bind_values(query, {});

    size_t count = 0;
    int result = sqlite3_step(statement);
    if( result == SQLITE_ROW )
    {
        count = static_cast<size_t>(sqlite3_column_int64(statement, 0));
    }
    else
    {
        throw circsim::common::StateError
        (
            sqlite3_errmsg(const_cast<sqlite3*>(_db_connection_obj))
        );
    }

    return count;
}


template bool ExternalStorage::contains(const circsim::components::Transistor& object) const;
template bool ExternalStorage::contains(const circsim::components::Wire& object) const;

template<class T>
bool ExternalStorage::contains(const T& object) const
{
    uint64_t id = object.id();

    const std::string query = "SELECT * FROM " + _table_name<T>() + " WHERE id=?;";
    SqliteStatement statement = _bind_values(query, { _to_sql_type(id) });

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

    return contains;
}


template bool ExternalStorage::contains_current(const circsim::components::Transistor& object) const;
template bool ExternalStorage::contains_current(const circsim::components::Wire& object) const;

template<class T>
bool ExternalStorage::contains_current(const T& object) const
{
    uint64_t id = object.id();

    const std::string query = "SELECT * FROM " + _table_name<T>() + " WHERE id=?;";
    SqliteStatement statement = _bind_values(query, { _to_sql_type(id) });

    bool contains_current = false;
    int result = sqlite3_step(statement);

    if( result == SQLITE_ROW )
    {
        T compare_to = _decode<T>(statement);
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


template circsim::components::Transistor ExternalStorage::get(const uint64_t id) const;
template circsim::components::Wire ExternalStorage::get(const uint64_t id) const;

template<class T>
T ExternalStorage::get(const uint64_t id) const
{
    T value;
    const std::string query = "SELECT * FROM " + _table_name<T>() + " WHERE id=?;";

    SqliteStatement statement = _bind_values(query, { _to_sql_type<uint64_t>(id) });
    int result = sqlite3_step(statement);

    if( result == SQLITE_ROW )
    {
        value = _decode<T>(statement);
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

