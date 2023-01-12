/**
 * @file flip-flop.cpp
 * @author your name (you@domain.com)
 * @brief Testflip flop for time-delayed feedback update.
 * @version 0.1
 * @date 2022-12-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <filesystem>
#include <vector>

// Test Framework Includes
#include <gtest/gtest.h>

// Project Includes
#include <circsim/components/Circuit.hpp>
#include <circsim/data/JsonParser.hpp>
#include <circsim/sim/Simulator.hpp>

class FlipFlopTest : public ::testing::Test
{
protected:

    const std::filesystem::path JSON_RELPATH = "flip-flop.json";

    const uint64_t WIRE_D_ID = 1;
    const uint64_t WIRE_CLK_ID = 3;
    const uint64_t WIRE_Q_ID = 4;
    const uint64_t WIRE_NOTQ_ID = 5;

    circsim::sim::Simulator* _simulator = nullptr;

    FlipFlopTest() = default;
    ~FlipFlopTest() = default;

    void SetUp() override;
    void TearDown() override;

    bool wire_status(const uint64_t wire_id);

};

void FlipFlopTest::SetUp()
{
    circsim::components::Circuit circuit;
    circuit = circsim::data::JsonParser::parse(JSON_RELPATH);

    _simulator = new circsim::sim::Simulator(circuit);
}

void FlipFlopTest::TearDown()
{
    delete _simulator;
}

bool FlipFlopTest::wire_status(const uint64_t wire_id)
{
    using Wire = circsim::components::Wire;

    Wire* wire = _simulator->circuit().get<Wire>(wire_id);
    return wire->high();
}

TEST_F(FlipFlopTest, FullComponentTest)
{
    using State = circsim::sim::Simulator::WireState;

    // Starting state, make sure output is steady-state
    _simulator->update_by_id(WIRE_D_ID, State::PULLED_LOW, false);
    _simulator->update_by_id(WIRE_CLK_ID, State::PULLED_HIGH, false);
    _simulator->update_all();

    EXPECT_FALSE(wire_status(WIRE_Q_ID));
    EXPECT_TRUE(wire_status(WIRE_NOTQ_ID));

    // Set CLK low, verify outputs
    _simulator->update_by_id(WIRE_CLK_ID, State::PULLED_LOW);

    EXPECT_FALSE(wire_status(WIRE_Q_ID));
    EXPECT_TRUE(wire_status(WIRE_NOTQ_ID));

    // Set D high, verify outputs
    _simulator->update_by_id(WIRE_D_ID, State::PULLED_HIGH);

    EXPECT_FALSE(wire_status(WIRE_Q_ID));
    EXPECT_TRUE(wire_status(WIRE_NOTQ_ID));

    // Set CLK high, verify outputs changed
    _simulator->update_by_id(WIRE_CLK_ID, State::PULLED_HIGH);

    EXPECT_TRUE(wire_status(WIRE_Q_ID));
    EXPECT_FALSE(wire_status(WIRE_NOTQ_ID));

    // Set D low, verify outputs changed
    _simulator->update_by_id(WIRE_D_ID, State::PULLED_LOW);

    EXPECT_FALSE(wire_status(WIRE_Q_ID));
    EXPECT_TRUE(wire_status(WIRE_NOTQ_ID));
}
