/**
 * @file ExternalStorage.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Test for type conversion with the external storage object.
 * @version 0.1
 * @date 2022-07-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// Framework Includes
#include <gtest/gtest.h>

// C++ Stdlib Includes
#include <cstdint>
#include <functional>
#include <limits>

// Project Includes
#define private public
#define protected public
#include <circsim/common/IndexError.hpp>
#include <circsim/data/ExternalStorage.hpp>
#undef private
#undef protected

using namespace circsim::data;


TEST(ExternalStorageTypes, UINT8)
{
    for(uint8_t i = 0; i < std::numeric_limits<uint8_t>::max(); i++)
    {
        uint8_t decoded_value;
        ExternalStorage::SqlValue value = ExternalStorage::_to_sql_type(i);

        ASSERT_NO_THROW(decoded_value = ExternalStorage::_from_sql_type<uint8_t>(value));
        EXPECT_EQ(i, decoded_value);
    }
}


const std::vector<uint64_t> size_values =
{
    0x00,
    0xDADA,
    0xFACE,
    0xCAFE,
    0xBABE,
    0xDADAFACE,
    0xCAFEBABE,
    std::numeric_limits<uint64_t>::max()
};

TEST(ExternalStorageTypes, UINT64)
{
    for( const uint64_t number : size_values )
    {
        uint64_t decoded_value;
        ExternalStorage::SqlValue value = ExternalStorage::_to_sql_type(number);

        ASSERT_NO_THROW(decoded_value = ExternalStorage::_from_sql_type<uint64_t>(value));
        EXPECT_EQ(number, decoded_value);
    }
}

TEST(ExternalStorageTypes, UINT64_LIST)
{
    std::vector<uint64_t> decoded_value;
    ExternalStorage::SqlValue value = ExternalStorage::_to_sql_type(size_values);

    ASSERT_NO_THROW(decoded_value = ExternalStorage::_from_sql_type<std::vector<uint64_t>>(value));
    EXPECT_EQ(size_values, decoded_value);
}


const std::vector<std::string> string_values =
{
    "",
    "Hello World!",
    "Special\tCharacters\n",
    "S"
};

TEST(ExternalStorageTypes, STRING)
{
    for( const std::string& str : string_values)
    {
        std::string decoded_value;
        ExternalStorage::SqlValue value = ExternalStorage::_to_sql_type(str);

        ASSERT_NO_THROW(decoded_value = ExternalStorage::_from_sql_type<std::string>(value));
        EXPECT_EQ(str, decoded_value);
    }
}

TEST(ExternalStorageTypes, STRING_LIST)
{
    std::vector<std::string> decoded_value;
    ExternalStorage::SqlValue value = ExternalStorage::_to_sql_type(string_values);

    ASSERT_NO_THROW(decoded_value = ExternalStorage::_from_sql_type<std::vector<std::string>>(value));
    EXPECT_EQ(string_values, decoded_value);
}


TEST(ExternalStorageTypes, BadConversion)
{
    std::vector<ExternalStorage::SqlValue> values = 
    {
        ExternalStorage::_to_sql_type<uint8_t>(0xFE),
        ExternalStorage::_to_sql_type<uint64_t>(0xCAFEBABE),
        ExternalStorage::_to_sql_type<std::vector<uint64_t>>({ 0xDADA, 0xFACE }),
        ExternalStorage::_to_sql_type<std::string>("Hello, World!"),
        ExternalStorage::_to_sql_type<std::vector<std::string>>({"Hello", ", ", "World", "!"})
    };

    // Try UINT8 conversion on all of them
    EXPECT_NO_THROW(ExternalStorage::_from_sql_type<uint8_t>(values[0]));
    EXPECT_THROW(ExternalStorage::_from_sql_type<uint8_t>(values[1]), circsim::common::IndexError);
    EXPECT_THROW(ExternalStorage::_from_sql_type<uint8_t>(values[2]), circsim::common::IndexError);
    EXPECT_THROW(ExternalStorage::_from_sql_type<uint8_t>(values[3]), circsim::common::IndexError);
    EXPECT_THROW(ExternalStorage::_from_sql_type<uint8_t>(values[4]), circsim::common::IndexError);
}
