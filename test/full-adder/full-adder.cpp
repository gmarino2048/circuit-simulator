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
#include <filesystem>

// Test Framework Includes
#include <gtest/gtest.h>

// Project Includes
#include <circsim/components/Circuit.hpp>
#include <circsim/data/JsonParser.hpp>
#include <circsim/sim/Simulator.hpp>

class FullAdderTest : public ::testing::Test
{
protected:

    const std::filesystem::path JSON_RELPATH = "full-adder.json";

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
    delete _simulator;
}


TEST_F(FullAdderTest, DummyTest)
{
    ASSERT_TRUE(true);
}