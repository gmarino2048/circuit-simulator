/**
 * @file full-adder.cpp
 * @author Guy Marino (gxm262@case.edu)
 * @brief Implementation for a test of the circuit simulator
 *        using a full adder.
 * @version 0.1
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <algorithm>
#include <filesystem>
#include <limits>
#include <utility>
#include <vector>

// Test Framework Includes
#include <gtest/gtest.h>

// Project Includes
#include <circsim/components/Register.hpp>
#include <circsim/components/Circuit.hpp>
#include <circsim/data/JsonParser.hpp>
#include <circsim/sim/Simulator.hpp>

class FullAdderTest : public ::testing::Test
{
protected:

    const std::filesystem::path JSON_RELPATH = "full-adder.json";

    const uint64_t REG_A_ID = 0;
    const uint64_t REG_B_ID = 1;

    circsim::sim::Simulator* _simulator = nullptr;

    FullAdderTest() = default;
    ~FullAdderTest() = default;

    void SetUp() override;
    void TearDown() override;

};


void FullAdderTest::SetUp()
{
    circsim::components::Circuit circuit;
    circuit = circsim::data::JsonParser::parse(JSON_RELPATH);

    _simulator = new circsim::sim::Simulator(circuit);

    // Quick and dirty hack to break in the circuit
    _simulator->update_by_register<uint8_t>(REG_A_ID, 0b11111111, true);
    _simulator->update_by_register<uint8_t>(REG_B_ID, 0b11111111, true);

    _simulator->update_by_register<uint8_t>(REG_A_ID, 0b00000000, true);
    _simulator->update_by_register<uint8_t>(REG_B_ID, 0b00000000, true);
}


void FullAdderTest::TearDown()
{
    circsim::components::Wire::RESET_CLASS();
    delete _simulator;
}


TEST_F(FullAdderTest, Level1_InversionTest)
{
    using Register = circsim::components::Register;

    const uint64_t REG_APRIME_ID = 2;
    const uint64_t REG_BPRIME_ID = 3;

    Register* not_a = nullptr;
    Register* not_b = nullptr;

    not_a = _simulator->circuit().get<Register>(REG_APRIME_ID);
    not_b = _simulator->circuit().get<Register>(REG_BPRIME_ID);

    for( uint8_t i = 0; i < std::numeric_limits<uint8_t>::max(); i++ )
    {
        _simulator->update_by_register<uint8_t>(REG_A_ID, i, false);
        _simulator->update_by_register<uint8_t>(REG_B_ID, i, false);

        _simulator->update_all();

        EXPECT_EQ(not_a->value_unsigned<uint8_t>(), uint8_t(~i));
        EXPECT_EQ(not_b->value_unsigned<uint8_t>(), uint8_t(~i));
    }
}


TEST_F(FullAdderTest, Level1_BaseAdderTest)
{
    using Wire = circsim::components::Wire;

    const uint64_t WIRE_OUT0_ID = 38;
    const uint64_t WIRE_C0_ID = 39;

    _simulator->update_by_register<uint8_t>(REG_A_ID, 0, false);
    _simulator->update_by_register<uint8_t>(REG_B_ID, 0, false);
    _simulator->update_all();

    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_OUT0_ID)->low());
    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_C0_ID)->low());

    _simulator->update_by_register(REG_A_ID, 1, true);

    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_OUT0_ID)->high());
    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_C0_ID)->low());

    _simulator->update_by_register(REG_B_ID, 1, true);
    
    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_OUT0_ID)->low());
    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_C0_ID)->high());

    _simulator->update_by_register(REG_A_ID, 0, true);

    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_OUT0_ID)->high());
    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_C0_ID)->low());
}
