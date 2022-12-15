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

    circsim::sim::Simulator* _simulator = nullptr;

    FlipFlopTest() = default;
    ~FlipFlopTest() = default;

    void SetUp() override;
    void TearDown() override;

};

void FlipFlopTest::SetUp()
{
    circsim::components::Circuit circuit;
    circuit = circsim::data::JsonParser::parse(JSON_RELPATH);

    _simulator = new circsim::sim::Simulator(circuit);
}

void FlipFlopTest::TearDown()
{
    circsim::components::Wire::RESET_CLASS();
    delete _simulator;
}

TEST_F(FlipFlopTest, DummyTestCase)
{
    EXPECT_TRUE(true);
}
