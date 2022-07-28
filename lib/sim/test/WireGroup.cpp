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
#include <circsim/data/InternalStorage.hpp>
#include <circsim/sim/WireGroup.hpp>

using Wire = circsim::components::Wire;
using Transistor = circsim::components::Transistor;
using Database = circsim::data::InternalStorage;
using WireGroup = circsim::sim::WireGroup;

TEST(WireGroup, NandTest)
{
    Database db = create_nand();

    Wire* gnd = db.get<Wire>(Wire::GND_ID());

    Wire* in_a = db.get<Wire>(1);
    Wire* in_b = db.get<Wire>(2);

    Wire* connector = db.get<Wire>(3);
    Wire* out = db.get<Wire>(4);

    db.get<Transistor>(1)->update_state(Wire::PULLED_LOW);
    db.get<Transistor>(2)->update_state(Wire::PULLED_LOW);

    WireGroup group(out->id(), db);

    EXPECT_TRUE(out->high());

    // Activate first transistor
    db.get<Transistor>(1)->update_state(Wire::PULLED_HIGH);

    group = WireGroup(out->id(), db);

    EXPECT_TRUE(out->high());
    EXPECT_TRUE(connector->high());

    // Activate second transistor
    db.get<Transistor>(2)->update_state(Wire::PULLED_HIGH);

    group = WireGroup(out->id(), db);

    EXPECT_TRUE(out->low());
    EXPECT_TRUE(connector->low());
}