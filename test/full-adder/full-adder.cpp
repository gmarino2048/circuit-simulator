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
    const uint64_t REG_OUT_ID = 4;

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

TEST_F(FullAdderTest, Level2_FullAdderTest)
{
    using Wire = circsim::components::Wire;

    const uint64_t WIRE_OUT1_ID = 53;
    const uint64_t WIRE_C1_ID = 61;

    // Base test, set all to 0
    _simulator->update_by_register<uint8_t>(REG_A_ID, 0x00, false);
    _simulator->update_by_register<uint8_t>(REG_B_ID, 0x00, false);
    _simulator->update_all();

    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_OUT1_ID)->low());
    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_C1_ID)->low());

    // Carry test, set to 2 1's
    _simulator->update_by_register<uint8_t>(REG_A_ID, 0x01, false);
    _simulator->update_by_register<uint8_t>(REG_B_ID, 0x01, false);
    _simulator->update_all();

    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_OUT1_ID)->high());
    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_C1_ID)->low());

    // Input Test, set to 2 and 1
    _simulator->update_by_register<uint8_t>(REG_A_ID, 0x02, false);
    _simulator->update_by_register<uint8_t>(REG_B_ID, 0x01, false);
    _simulator->update_all();

    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_OUT1_ID)->high());
    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_C1_ID)->low());

    _simulator->update_by_register<uint8_t>(REG_A_ID, 0x01, false);
    _simulator->update_by_register<uint8_t>(REG_B_ID, 0x02, false);
    _simulator->update_all();

    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_OUT1_ID)->high());
    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_C1_ID)->low());

    // Carry out test, set to 2 2's
    _simulator->update_by_register<uint8_t>(REG_A_ID, 0x02, false);
    _simulator->update_by_register<uint8_t>(REG_B_ID, 0x02, false);
    _simulator->update_all();

    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_OUT1_ID)->low());
    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_C1_ID)->high());

    // Combined input & output test, set to 2 3's
    _simulator->update_by_register<uint8_t>(REG_A_ID, 0x03, false);
    _simulator->update_by_register<uint8_t>(REG_B_ID, 0x03, false);
    _simulator->update_all();

    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_OUT1_ID)->high());
    EXPECT_TRUE(_simulator->circuit().get<Wire>(WIRE_C1_ID)->high());
}

TEST_F(FullAdderTest, Level3_HalfwayPointFidelityTest)
{
    using Register = circsim::components::Register;

    for( uint8_t in_a = 0b0000; in_a < 0b1000; in_a++ )
    {
        for( uint8_t in_b = 0b0000; in_b < 0b1000; in_b++ )
        {
            uint8_t expected = in_a + in_b;

            _simulator->update_by_register<uint8_t>(REG_A_ID, in_a, false);
            _simulator->update_by_register<uint8_t>(REG_B_ID, in_b, false);
            _simulator->update_all();

            Register* output_register = _simulator->circuit().get<Register>(REG_OUT_ID);
            uint8_t out_value = output_register->value_unsigned<uint8_t>();

            EXPECT_EQ(out_value, expected);
        }
    }
}
