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

    const std::filesystem::path CPU_6502_RELPATH = "6502.json";
    const std::filesystem::path CPU_6800_RELPATH = "6800.json";

    circsim::sim::Simulator* _simulator = nullptr;

    CpuTest() = default;
    ~CpuTest() = default;

    void TearDown();

};

void CpuTest::TearDown()
{
    delete _simulator;
}

TEST_F(CpuTest, Test6502)
{
    circsim::components::Circuit circuit;
    ASSERT_NO_FATAL_FAILURE(circuit = circsim::data::JsonParser::parse(CPU_6502_RELPATH));

    ASSERT_NO_FATAL_FAILURE(_simulator = new circsim::sim::Simulator(circuit));
}

TEST_F(CpuTest, Test6800)
{
    circsim::components::Circuit circuit;
    ASSERT_NO_FATAL_FAILURE(circuit = circsim::data::JsonParser::parse(CPU_6800_RELPATH));

    ASSERT_NO_FATAL_FAILURE(_simulator = new circsim::sim::Simulator(circuit));
}