/**
 * @file Transistor.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Unit test file for the Transistor object
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// Test framework include
#include <gtest/gtest.h>

// C++ stdlib includes
#include <vector>

// Project includes
#include <circsim/components/Wire.hpp>
#include <circsim/components/Transistor.hpp>

using Wire = circsim::components::Wire;
using Transistor = circsim::components::Transistor;

class TransistorTest : public ::testing::Test
{
protected:

    static std::vector<Wire::State> _wire_states;

    TransistorTest() = default;
    ~TransistorTest() = default;

};


std::vector<Wire::State> TransistorTest::_wire_states =
{
    Wire::GROUNDED,
    Wire::PULLED_LOW,
    Wire::FLOATING_LOW,
    Wire::FLOATING,
    Wire::FLOATING_HIGH,
    Wire::PULLED_HIGH,
    Wire::HIGH
};


TEST_F(TransistorTest, DefaultConstructor)
{
    Transistor transistor;

    EXPECT_EQ(transistor.id(), -1);
    EXPECT_TRUE(transistor.name().empty());
    EXPECT_EQ(transistor.type(), Transistor::NMOS);
    EXPECT_EQ(transistor.current_state(), Transistor::OFF);

    EXPECT_EQ(transistor.gate(), 0);
    EXPECT_EQ(transistor.source(), 0);
    EXPECT_EQ(transistor.drain(), 0);
}


TEST_F(TransistorTest, NormalConstructor)
{
    Transistor transistor
    (
        4,
        "Test",
        5, 6, 7,
        Transistor::PMOS
    );

    EXPECT_EQ(transistor.id(), 4);
    EXPECT_EQ(transistor.name(), "Test");
    EXPECT_EQ(transistor.type(), Transistor::PMOS);
    EXPECT_EQ(transistor.current_state(), Transistor::OFF);

    EXPECT_EQ(transistor.gate(), 5);
    EXPECT_EQ(transistor.source(), 6);
    EXPECT_EQ(transistor.drain(), 7);
}


TEST_F(TransistorTest, NmosSwitch)
{
    Transistor transistor(1, 0, 0, 0, Transistor::NMOS);

    for( const Wire::State state : _wire_states )
    {
        bool switched = transistor.update_state(state);

        if( state < Wire::FLOATING_HIGH )
        {
            EXPECT_FALSE(switched);
            EXPECT_EQ(transistor.current_state(), Transistor::OFF);
        }
        else if( state == Wire::FLOATING_HIGH )
        {
            EXPECT_TRUE(switched);
            EXPECT_EQ(transistor.current_state(), Transistor::ON);
        }
        else
        {
            EXPECT_FALSE(switched);
            EXPECT_EQ(transistor.current_state(), Transistor::ON);
        }
    }
}


TEST_F(TransistorTest, PmosSwitch)
{
    Transistor transistor(1, 0, 0, 0, Transistor::PMOS);

    for( const Wire::State state : _wire_states )
    {
        bool switched = transistor.update_state(state);

        if( state == Wire::GROUNDED )
        {
            EXPECT_TRUE(switched);
            EXPECT_EQ(transistor.current_state(), Transistor::ON);
        }
        else if( state < Wire::FLOATING_HIGH )
        {
            EXPECT_FALSE(switched);
            EXPECT_EQ(transistor.current_state(), Transistor::ON);
        }
        else if( state == Wire::FLOATING_HIGH )
        {
            EXPECT_TRUE(switched);
            EXPECT_EQ(transistor.current_state(), Transistor::OFF);
        }
        else
        {
            EXPECT_FALSE(switched);
            EXPECT_EQ(transistor.current_state(), Transistor::OFF);
        }
    }
}
