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

#include <algorithm>
#include <cstring>
#include <limits>

#include <circsim/common/EndianOperations.hpp>
#include <circsim/common/ValueError.hpp>
#include <circsim/data/ExternalStorage.hpp>

using namespace circsim::data;
using SqlValue = ExternalStorage::SqlValue;

// Template Initialization of ExternalStorage::_to_sql_type
template<>
SqlValue ExternalStorage::_to_sql_type(const uint8_t value);

template<>
SqlValue ExternalStorage::_to_sql_type(const size_t value);

template<>
SqlValue ExternalStorage::_to_sql_type(const std::vector<size_t>& value);

template<>
SqlValue ExternalStorage::_to_sql_type(const std::string& value);

template<>
SqlValue ExternalStorage::_to_sql_type(const std::vector<std::string>& value);


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
            const size_t* value_ptr = &value;
            int64_t converted_value = *(reinterpret_cast<const int64_t*>(value_ptr));

            return SqlValue(converted_value);
        }
    }
}

template<>
SqlValue ExternalStorage::_to_sql_type(const std::vector<size_t>& value)
{
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
SqlValue ExternalStorage::_to_sql_type(const std::string& value)
{
    // No need to do type conversion
    return SqlValue(value);
}

template<>
SqlValue ExternalStorage::_to_sql_type(const std::vector<std::string>& value)
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
    byte_array.resize(byte_count);

    for(const std::string& str : value)
    {
        std::copy
        (
            str.begin(),
            str.end(),
            std::back_inserter(byte_array)
        );
        byte_array.push_back('\0');
    }

    return SqlValue(byte_array);
}
