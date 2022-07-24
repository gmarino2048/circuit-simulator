/**
 * @file EndianOperations.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Implement some simple endian operations.
 * @version 0.1
 * @date 2022-07-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <cstddef>
#include <cstdint>

#include <circsim/common/EndianOperations.hpp>

using namespace circsim::common;

// Template initialization of EndianOperations::_byteswap
template int16_t EndianOperations::_byteswap(const int16_t value);
template int32_t EndianOperations::_byteswap(const int32_t value);
template int64_t EndianOperations::_byteswap(const int64_t value);

template uint16_t EndianOperations::_byteswap(const uint16_t value);
template uint32_t EndianOperations::_byteswap(const uint32_t value);
template uint64_t EndianOperations::_byteswap(const uint64_t value);

template size_t EndianOperations::_byteswap(const size_t value);

// Template initialization of EndianOperations::host_to_big_endian
template int16_t EndianOperations::host_to_big_endian(const int16_t value);
template int32_t EndianOperations::host_to_big_endian(const int32_t value);
template int64_t EndianOperations::host_to_big_endian(const int64_t value);

template uint16_t EndianOperations::host_to_big_endian(const uint16_t value);
template uint32_t EndianOperations::host_to_big_endian(const uint32_t value);
template uint64_t EndianOperations::host_to_big_endian(const uint64_t value);

template size_t EndianOperations::host_to_big_endian(const size_t value);

// Template initialization of EndianOperations::big_endian_to_host
template int16_t EndianOperations::big_endian_to_host(const int16_t value);
template int32_t EndianOperations::big_endian_to_host(const int32_t value);
template int64_t EndianOperations::big_endian_to_host(const int64_t value);

template uint16_t EndianOperations::big_endian_to_host(const uint16_t value);
template uint32_t EndianOperations::big_endian_to_host(const uint32_t value);
template uint64_t EndianOperations::big_endian_to_host(const uint64_t value);

template size_t EndianOperations::big_endian_to_host(const size_t value);


EndianOperations::Endianness EndianOperations::_determine_endianness()
{
    const uint16_t check = 1;
    const uint8_t* check_ptr = reinterpret_cast<const uint8_t*>(&check);

    if( *check_ptr )
    {
        return Endianness::LITTLE;
    }
    else
    {
        return Endianness::BIG;
    }
}


template<class T>
T EndianOperations::_byteswap(const T value)
{
    T new_value;
    size_t byte_count = sizeof(T);

    uint8_t* source_ptr = (uint8_t*)&value;
    uint8_t* dest_ptr = (uint8_t*)&new_value;

    dest_ptr = dest_ptr + byte_count - 1;

    for(size_t i = 0; i < byte_count; i++)
    {
        *(dest_ptr - i) = *(source_ptr + i);
    }

    return new_value;
}


template<class T>
T EndianOperations::host_to_big_endian(const T value)
{
    if( _determine_endianness() == Endianness::BIG )
    {
        return value;
    }
    else
    {
        return _byteswap(value);
    }
}


template<class T>
T EndianOperations::big_endian_to_host(const T value)
{
    if( _determine_endianness() == Endianness::BIG )
    {
        return value;
    }
    else
    {
        return _byteswap(value);
    }
}


EndianOperations::~EndianOperations() = default;
