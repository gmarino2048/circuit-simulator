/**
 * @file Transistor.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Test file for storing different transistor objects
 * @version 0.1
 * @date 2022-08-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <vector>

// Test Framework Includes
#include <gtest/gtest.h>

// Project Includes
#include <circsim/common/StateError.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/data/ExternalStorage.hpp>

using namespace circsim::common;
using namespace circsim::components;
using namespace circsim::data;

class TransistorStorage : public ::testing::Test
{
protected:

    ExternalStorage* _storage = nullptr;
    std::vector<Transistor> _transistors;

    void SetUp() override
    {
        _storage = new ExternalStorage();

        _transistors =
        {
            Transistor(13, 0xCAFE, 0xBABE, 0xFACE, Transistor::Type::PMOS),
            Transistor(45, "test", 0xFACE, 0xCAFE, 0xBABE, Transistor::Type::NMOS)
        };
    }

    void TearDown() override
    {
        delete _storage;
    }

};


TEST_F(TransistorStorage, Fidelity)
{
    Transistor t1 = _transistors[1];
    Transistor t2;

    ASSERT_NO_THROW(_storage->add_component(t1));
    ASSERT_NO_THROW(t2 = _storage->get<Transistor>(t1.id()));

    EXPECT_EQ(t1, t2);
}


TEST_F(TransistorStorage, FidelityNoName)
{
    Transistor t1 = _transistors[0];
    Transistor t2;

    ASSERT_NO_THROW(_storage->add_component(t1));
    ASSERT_NO_THROW(t2 = _storage->get<Transistor>(t1.id()));

    EXPECT_EQ(t1, t2);
}

TEST_F(TransistorStorage, GetAll)
{
    for( const Transistor& t : _transistors )
    {
        ASSERT_NO_THROW(_storage->add_component(t));
    }

    std::vector<Transistor> result;
    ASSERT_NO_THROW(result = _storage->get_all<Transistor>());

    EXPECT_EQ(_transistors, result);
}


TEST_F(TransistorStorage, DoubleInsertion)
{
    Transistor t1 = _transistors[0];

    ASSERT_NO_THROW(_storage->add_component(t1));
    EXPECT_THROW(_storage->add_component(t1), StateError);

    auto ts = _storage->get_all<Transistor>();
}
