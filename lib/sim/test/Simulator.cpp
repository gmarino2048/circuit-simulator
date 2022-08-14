/**
 * @file Simulator.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Test file for the Simulator Object as a whole.
 * @version 0.1
 * @date 2022-02-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// Test framework Includes
#include <Gates.hpp>
#include <gtest/gtest.h>

// C++ Stdlib Includes
#include <stdexcept>

// Project includes
#include <circsim/sim/Simulator.hpp>

using namespace circsim::components;
using namespace circsim::data;
using namespace circsim::sim;

class SimulatorTest : public ::testing::Test
{
protected:

    Simulator *_simulator;

    SimulatorTest() = default;
    ~SimulatorTest() override = default;

    void SetUp() override;
    void TearDown() override;

    bool decode_output(const Wire *wire);
    Wire::State encode_input(const bool input);

};

void SimulatorTest::SetUp()
{
    // Do nothing
}

void SimulatorTest::TearDown()
{
    delete _simulator;
    Wire::RESET_CLASS();
}

bool SimulatorTest::decode_output(const Wire *wire)
{
    if( wire == nullptr )
    {
        throw std::runtime_error("Null wire provided");
    }

    return wire->high();
}

Wire::State SimulatorTest::encode_input(const bool input)
{
    return input ? Wire::State::PULLED_HIGH : Wire::State::PULLED_LOW;
}


TEST_F(SimulatorTest, TestNOR)
{
    // Set up simulator
    Circuit circuit = create_nor();
    _simulator = new Simulator(circuit);

    bool input_options[2] = { true, false };

    for( const bool in_a : input_options )
    {
        for( const bool in_b : input_options )
        {
            // Set wire conditions
            _simulator->update_by_name
            (
                "IN_A",
                encode_input(in_a),
                false
            );

            _simulator->update_by_name
            (
                "IN_B",
                encode_input(in_b),
                false
            );

            // Update internal state of simulator
            _simulator->update_all();

            // Get output of wire
            Wire *out_wire = _simulator->circuit().find<Wire>("OUT");
            bool out = decode_output(out_wire);

            EXPECT_EQ( !(in_a || in_b), out );
        }
    }
}

TEST_F(SimulatorTest, TestNand)
{
    // Set up simulator
    Circuit circuit = create_nand();
    _simulator = new Simulator(circuit);

    bool input_options[2] = { true, false };

    for( const bool in_a : input_options )
    {
        for( const bool in_b : input_options )
        {
            // Set wire conditions
            _simulator->update_by_name
            (
                "IN_A",
                encode_input(in_a),
                false
            );

            _simulator->update_by_name
            (
                "IN_B",
                encode_input(in_b),
                false
            );

            // Update internal state of simulator
            _simulator->update_all();

            // Get output of wire
            Wire *out_wire = _simulator->circuit().find<Wire>("OUT");
            bool out = decode_output(out_wire);

            EXPECT_EQ( !(in_a && in_b), out );
        }
    }
}