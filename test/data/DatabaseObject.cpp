/**
 * @file DatabaseObject.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Test methods for the database object
 * @version 0.1
 * @date 2022-03-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// Framework Include
#include <gtest/gtest.h>

// C++ Stdlib Includes
#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

// Project Includes
#include <circsim/data/DatabaseObject.hpp>

using namespace circsim::data;
using DbType = DatabaseObject::DbType;
using DbValue = DatabaseObject::DbValue;

TEST(DatabaseObject, TestBoolConversion)
{
    DbValue output_true = DatabaseObject::format_value(true);
    DbValue output_false = DatabaseObject::format_value(false);

    EXPECT_EQ(output_true.type, DbType::DBT_INTEGER);
    EXPECT_EQ(output_false.type, DbType::DBT_INTEGER);

    EXPECT_EQ(output_true.value, "1");
    EXPECT_EQ(output_false.value, "0");
}


TEST(DatabaseObject, TestSizeConversion)
{
    std::vector<size_t> test_objects =
    {
        1, 1, 2, 3, 5, 8, 13, 21, 34
    };

    std::vector<std::string> expected_outputs =
    {
        "1", "1", "2", "3", "5", "8", "13", "21", "34"
    };

    ASSERT_EQ(test_objects.size(), expected_outputs.size());

    std::vector<std::string> outputs;
    for ( const size_t input : test_objects )
    {
        DbValue output = DatabaseObject::format_value(input);
        outputs.push_back(output.value);

        EXPECT_EQ(output.type, DbType::DBT_INTEGER);
    }

    ASSERT_EQ(outputs, expected_outputs);
}


TEST(DatabaseObject, TestGoodStringConversion)
{
    std::vector<std::string> inputs =
    {
        "Hello",
        "World",
        "This is a complete sentence!",
        "wire1",
        "wire2",
        "transistor3",
        "transistor4"
    };

    std::vector<std::string> expected_outputs(inputs.size());
    std::transform
    (
        inputs.begin(),
        inputs.end(),
        expected_outputs.begin(),
        [](const std::string &input)
        {
            std::stringstream stream;
            stream << "\"" << input << "\"";

            return stream.str();
        }
    );

    std::vector<std::string> outputs;
    for ( const std::string &input : inputs )
    {
        DbValue output = DatabaseObject::format_value(input);
        outputs.push_back(output.value);

        EXPECT_EQ(output.type, DbType::DBT_TEXT);
    }

    EXPECT_EQ(outputs, expected_outputs);
}
