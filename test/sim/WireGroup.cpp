
#include <gtest/gtest.h>

#include <Gates.hpp>

#include <circsim/components/Wire.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/data/InternalDatabase.hpp>
#include <circsim/sim/WireGroup.hpp>

using Wire = circsim::components::Wire;
using Transistor = circsim::components::Transistor;
using Database = circsim::data::InternalDatabase;
using WireGroup = circsim::sim::WireGroup;

TEST(WireGroup, NandTest)
{
    Database db = create_nand();

    Wire* gnd = db.get_wire(Wire::GND_ID());

    Wire* in_a = db.get_wire(1);
    Wire* in_b = db.get_wire(2);

    Wire* connector = db.get_wire(3);
    Wire* out = db.get_wire(4);

    db.get_transistor(1)->update_state(Wire::PULLED_LOW);
    db.get_transistor(2)->update_state(Wire::PULLED_LOW);

    WireGroup group(out->id(), db);

    EXPECT_TRUE(out->high());

    // Activate first transistor
    db.get_transistor(1)->update_state(Wire::PULLED_HIGH);

    group = WireGroup(out->id(), db);

    EXPECT_TRUE(out->high());
    EXPECT_TRUE(connector->high());

    // Activate second transistor
    db.get_transistor(2)->update_state(Wire::PULLED_HIGH);

    group = WireGroup(out->id(), db);

    EXPECT_TRUE(out->low());
    EXPECT_TRUE(connector->low());
}