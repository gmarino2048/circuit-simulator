/**
 * @file WireGroup.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Unit test file for the wire group object
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <gtest/gtest.h>

#include <Gates.hpp>

#include <circsim/components/Wire.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/sim/WireGroup.hpp>

using Wire = circsim::components::Wire;
using Transistor = circsim::components::Transistor;
using Circuit = circsim::components::Circuit;
using WireGroup = circsim::sim::WireGroup;

TEST(WireGroup, NandTest)
{
    Circuit circuit = create_nand();

    Wire* gnd = circuit.get<Wire>(Wire::GND_ID());

    Wire* in_a = circuit.get<Wire>(1);
    Wire* in_b = circuit.get<Wire>(2);

    Wire* connector = circuit.get<Wire>(3);
    Wire* out = circuit.get<Wire>(4);

    circuit.get<Transistor>(1)->update_state(Wire::PULLED_LOW);
    circuit.get<Transistor>(2)->update_state(Wire::PULLED_LOW);

    WireGroup group(out->id(), circuit);

    EXPECT_TRUE(out->high());

    // Activate first transistor
    circuit.get<Transistor>(1)->update_state(Wire::PULLED_HIGH);

    group = WireGroup(out->id(), circuit);

    EXPECT_TRUE(out->high());
    EXPECT_TRUE(connector->high());

    // Activate second transistor
    circuit.get<Transistor>(2)->update_state(Wire::PULLED_HIGH);

    group = WireGroup(out->id(), circuit);

    EXPECT_TRUE(out->low());
    EXPECT_TRUE(connector->low());
}