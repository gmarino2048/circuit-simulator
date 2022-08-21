/**
 * @file JsonParser.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief This test module tests functionality for the
 *        JSON parser.
 * @version 0.1
 * @date 2022-08-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <vector>

// Library Includes
#include <boost/json.hpp>

// Test Framework Includes
#include <gtest/gtest.h>

// Project Includes
#include <circsim/common/ValueError.hpp>
#include <circsim/components/Circuit.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>

#define protected public
#define private public
#include <circsim/data/JsonParser.hpp>
#undef private
#undef protected

using namespace circsim::common;
using namespace circsim::components;
using namespace circsim::data;


class JsonParserTest : public ::testing::Test
{
protected:

    std::vector<std::string> _test_keys;
    boost::json::object _test_values;

    template<class T>
    T convert_value(const std::string& index);

    void SetUp();

    void TearDown();

};


template<class T>
T JsonParserTest::convert_value(const std::string&index)
{
    return JsonParser::_convert_object<T>(_test_values.at(index));
}


void JsonParserTest::SetUp()
{
    _test_keys = 
    { 
        "int",
        "float",
        "str",
        "int_arr",
        "str_arr",
        "trans_types",
        "wire_special",
        "wire_pulled"
    };

    std::string json_string =
    "{"
    "   \"int\": 16,"
    "   \"float\": 12.5,"
    "   \"str\": \"Hello World\","
    "   \"int_arr\": ["
    "       0,"
    "       32,"
    "       64,"
    "       18446744073709551615"
    "   ],"
    "   \"str_arr\": ["
    "       \"\","
    "       \"Hello World\","
    "       \" \\\" Character Test \\\" \""
    "   ],"
    "   \"trans_types\": ["
    "       \"NMOS\","
    "       \"PMOS\","
    "       \"Invalid\""
    "   ],"
    "   \"wire_special\": ["
    "       \"VCC\","
    "       \"GND\","
    "       \"NONE\","
    "       \"Invalid\""
    "   ],"
    "   \"wire_pulled\": ["
    "       \"HIGH\","
    "       \"LOW\","
    "       \"NONE\","
    "       \"Invalid\""
    "   ]"
    "}";

    _test_values = boost::json::parse(json_string).as_object();
}


void JsonParserTest::TearDown()
{
    // No content
}


TEST_F(JsonParserTest, SimpleInt)
{
    uint64_t value;

    for( const std::string& key : _test_keys )
    {
        if( key == "int")
        {
            ASSERT_NO_THROW(value = convert_value<uint64_t>(key));
        }
        else
        {
            EXPECT_THROW(convert_value<uint64_t>(key), ValueError);
        }
    }

    EXPECT_EQ(value, 16);
}


TEST_F(JsonParserTest, SimpleString)
{
    std::string value;

    for( const std::string& key : _test_keys )
    {
        if( key == "str" )
        {
            ASSERT_NO_THROW(value = convert_value<std::string>(key));
        }
        else
        {
            EXPECT_THROW(convert_value<std::string>(key), ValueError);
        }
    }

    EXPECT_EQ(value, "Hello World");
}


TEST_F(JsonParserTest, IntArray)
{
    std::vector<uint64_t> value;

    for( const std::string& key : _test_keys )
    {
        if( key == "int_arr" )
        {
            ASSERT_NO_THROW(value = convert_value<std::vector<uint64_t>>(key));
        }
        else
        {
            EXPECT_THROW(convert_value<std::vector<uint64_t>>(key), ValueError);
        }
    }

    std::vector<uint64_t> expected = { 0U, 32U, 64U, 18446744073709551615U };
    EXPECT_EQ(value, expected);
}


TEST_F(JsonParserTest, StringArray)
{
    std::vector<std::string> value;

    ASSERT_NO_THROW(value = convert_value<std::vector<std::string>>("str_arr"));

    std::vector<std::string> expected =
    {
        "",
        "Hello World",
        " \" Character Test \" "
    };

    EXPECT_EQ(value, expected);
}


TEST_F(JsonParserTest, TransistorType)
{
    boost::json::array types;
    ASSERT_NO_THROW(types = _test_values["trans_types"].as_array());

    for( const boost::json::value& value : types )
    {
        if( value.as_string() == "Invalid" )
        {
            EXPECT_THROW(JsonParser::_convert_object<Transistor::Type>(value), ValueError);
        }
        else
        {
            EXPECT_NO_THROW(JsonParser::_convert_object<Transistor::Type>(value));
        }
    }
}


TEST_F(JsonParserTest, SpecialWireType)
{
    boost::json::array types;
    ASSERT_NO_THROW(types = _test_values["wire_special"].as_array());

    for( const boost::json::value& value : types )
    {
        if( value.as_string() == "Invalid" )
        {
            EXPECT_THROW(JsonParser::_convert_object<Wire::SpecialWireType>(value), ValueError);
        }
        else
        {
            EXPECT_NO_THROW(JsonParser::_convert_object<Wire::SpecialWireType>(value));
        }
    }
}


TEST_F(JsonParserTest, WirePulled)
{
    boost::json::array types;
    ASSERT_NO_THROW(types = _test_values["wire_pulled"].as_array());

    for( const boost::json::value& value : types )
    {
        if( value.as_string() == "Invalid" )
        {
            EXPECT_THROW(JsonParser::_convert_object<Wire::PulledStatus>(value), ValueError);
        }
        else
        {
            EXPECT_NO_THROW(JsonParser::_convert_object<Wire::PulledStatus>(value));
        }
    }
}
