/**
 * @file main.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
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

class CpuTest : public ::testing::Test
{
protected:

    const std::filesystem::path JSON_RELPATH = "6502.json";

    circsim::sim::Simulator* _simulator = nullptr;

    CpuTest() = default;
    ~CpuTest() = default;

    void SetUp() override;
    void TearDown() override;

};

void CpuTest::SetUp()
{
    circsim::components::Circuit circuit;
    circuit = circsim::data::JsonParser::parse(JSON_RELPATH);

    _simulator = new circsim::sim::Simulator(circuit);
}

void CpuTest::TearDown()
{
    delete _simulator;
}

TEST_F(CpuTest, SimpleTest)
{
    EXPECT_TRUE(_simulator != nullptr);
}