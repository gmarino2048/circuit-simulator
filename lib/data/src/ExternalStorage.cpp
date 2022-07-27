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

// Project Includes
#include <circsim/common/EndianOperations.hpp>
#include <circsim/common/IndexError.hpp>
#include <circsim/common/ValueError.hpp>
#include <circsim/data/ExternalStorage.hpp>

using namespace circsim::data;
using SqlValue = ExternalStorage::SqlValue;

// File constants
const char NULL_TERMINATOR = '\0';

// Template Initialization of ExternalStorage::_to_sql_type
template<>
SqlValue ExternalStorage::_to_sql_type(const uint8_t value);

template<>
SqlValue ExternalStorage::_to_sql_type(const size_t value);

template<>
SqlValue ExternalStorage::_to_sql_type(const std::vector<size_t> value);

template<>
SqlValue ExternalStorage::_to_sql_type(const std::string value);

template<>
SqlValue ExternalStorage::_to_sql_type(const std::vector<std::string> value);


// Template Initialization of ExternalStorage::_from_sql_type
template<>
uint8_t ExternalStorage::_from_sql_type(const SqlValue& value);

template<>
size_t ExternalStorage::_from_sql_type(const SqlValue& value);

template<>
std::vector<size_t> ExternalStorage::_from_sql_type(const SqlValue& value);

template<>
std::string ExternalStorage::_from_sql_type(const SqlValue& value);

template<>
std::vector<std::string> ExternalStorage::_from_sql_type(const SqlValue& value);


// Implementation of ExternalStorage::_to_sql_type
template<>
SqlValue ExternalStorage::_to_sql_type(const uint8_t value)
{
    // Simple cast is fine since size goes up
    return SqlValue(static_cast<int32_t>(value));
}

template<>
SqlValue ExternalStorage::_to_sql_type(const size_t value)
{
    if constexpr( sizeof(size_t) == sizeof(int64_t) )
    {
        // Do a bitwise conversion to avoid overflow issues
        const size_t* value_ptr = &value;
        int64_t converted_value = *(reinterpret_cast<const int64_t*>(value_ptr));

        return SqlValue(converted_value);
    }
    else if( sizeof(size_t) < sizeof(int64_t) )
    {
        // We don't need to worry about overflow in this case
        return SqlValue(static_cast<int64_t>(value));
    }
    else
    {
        // size_t exceeds 64 bits
        // This should theoretically never happen on current systems
        if( value > std::numeric_limits<uint64_t>::max() )
        {
            // This means that the value itself takes up more than 8 bytes
            throw circsim::common::ValueError
            (
                "Provided value \"" + std::to_string(value) + "\" exceeds INT64 storage capacity."
            );
        }
        else
        {
            // Do a bitwise conversion to avoid overflow issues
            const uint64_t value_long = static_cast<uint64_t>(value);
            const uint64_t* value_ptr = &value_long;
            int64_t converted_value = *(reinterpret_cast<const int64_t*>(value_ptr));

            return SqlValue(converted_value);
        }
    }
}

template<>
SqlValue ExternalStorage::_to_sql_type(const std::vector<size_t> value)
{
    // NOTE: This method assumes that size_t is always the same size
    // Convert the internal vector buffer to a void pointer
    std::vector<size_t> be_values;
    be_values.reserve(value.size());

    std::transform
    (
        value.begin(),
        value.end(),
        std::back_inserter(be_values),
        [](size_t number_value)
        {
            return circsim::common::EndianOperations::host_to_big_endian(number_value);
        }
    );

    size_t byte_count = sizeof(size_t) * be_values.size();
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
size_t ExternalStorage::_from_sql_type(const SqlValue& value) try
{
    const int64_t num_value = std::get<int64_t>(value);

    if constexpr( sizeof(size_t) == sizeof(int64_t) )
    {
        // Direct pointer-cast to size_t so we don't lose resolution
        size_t converted_value = *(reinterpret_cast<const size_t*>(&num_value));
        return converted_value;
    }
    else if( sizeof(size_t) < sizeof(int64_t) )
    {
        // Need to check for overflow errors here
        if( num_value > std::numeric_limits<uint64_t>::max() )
        {
            // This means that the value itself takes up more than size_t can handle
            throw circsim::common::ValueError
            (
                "Provided value \"" + std::to_string(num_value) + "\" exceeds SIZE storage capacity."
            );
        }
        else
        {
            // Do a bitwise conversion to avoid overflow issues
            const int64_t* value_ptr = &num_value;
            uint64_t converted_value = *(reinterpret_cast<const uint64_t*>(value_ptr));

            return static_cast<size_t>(converted_value);
        }
    }
    else
    {
        return static_cast<size_t>(num_value);
    }
}
catch( const std::bad_variant_access& )
{
    throw circsim::common::IndexError
    (
        "SQL type does not contain int64, required for conversion to size."
    );
}

template<>
std::vector<size_t> ExternalStorage::_from_sql_type(const SqlValue& value) try
{
    const std::vector<uint8_t>& buffer = std::get<std::vector<uint8_t>>(value);
    size_t buffer_byte_count = buffer.size();

    if( buffer_byte_count % sizeof(size_t) != 0 )
    {
        throw circsim::common::ValueError
        (
            "Size of buffer (" + std::to_string(buffer_byte_count) + 
            ") does not align with size of size_t (" + std::to_string(sizeof(size_t)) + ")."
        );
    }

    size_t value_count = buffer_byte_count / sizeof(size_t);
    std::vector<size_t> values(value_count);

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
        [](const size_t value)
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
                string_char_count * sizeof(value[0])
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

