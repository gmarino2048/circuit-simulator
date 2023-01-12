/**
 * @file Wire.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Tests the database implementation of the wire object
 * @version 0.1
 * @date 2022-08-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <algorithm>
#include <cstdint>
#include <limits>
#include <vector>

// Test Framework Includes
#include <gtest/gtest.h>

// Project Includes
#include <circsim/common/StateError.hpp>
#include <circsim/components/Wire.hpp>
#include <circsim/data/ExternalStorage.hpp>

using namespace circsim::components;
using namespace circsim::data;

class WireStorage : public ::testing::Test
{
protected:

    ExternalStorage *_storage;
    std::vector<Wire> _wires;

    void SetUp()
    {
        _storage = new ExternalStorage();

        _wires =
        {
            Wire
            (
                0,
                "wire1",
                Wire::PulledStatus::PS_HIGH,
                { 1, 2, 3 },
                { 4, 5, 6 }
            ),
            Wire
            (
                1,
                Wire::SpecialWireType::SW_GND,
                { 3, 6, 9 },
                { 2, 4, 8 }
            ),
            Wire
            (
                84,
                "wire2",
                Wire::PulledStatus::PS_LOW,
                {},
                { 1, 2, 3, 4, 5, 6 }
            ),
            Wire
            (
                std::numeric_limits<uint64_t>::max(),
                "",
                Wire::PulledStatus::PS_NONE,
                { 0, std::numeric_limits<uint64_t>::max() },
                { 3, 4, 5, 6, 7, 8, 9 }
            )
        };
    }

    void TearDown()
    {
        delete _storage;
    }
};


TEST_F(WireStorage, Count)
{
    for( const Wire& wire : _wires )
    {
        ASSERT_NO_THROW(_storage->add_component(wire));
    }

    EXPECT_EQ(_storage->count<Wire>(), _wires.size());
}


TEST_F(WireStorage, ContainsCurrentState)
{
    Wire wire = _wires[0];
    ASSERT_NO_THROW(_storage->add_component(wire));

    wire.state(Wire::State::PULLED_HIGH);
    EXPECT_TRUE(_storage->contains_current(wire));
}


TEST_F(WireStorage, ElementFidelity)
{
    for( const Wire& wire : _wires )
    {
        ASSERT_NO_THROW(_storage->add_component(wire));
        EXPECT_TRUE(_storage->contains_current(wire));
    }
}


TEST_F(WireStorage, DuplicateAddition)
{
    Wire wire = _wires[0];

    ASSERT_NO_THROW(_storage->add_component(wire));
    EXPECT_THROW(_storage->add_component(wire), circsim::common::StateError);
}


TEST_F(WireStorage, UpdateInsertion)
{
    Wire wire = _wires[0];

    ASSERT_NO_THROW(_storage->update_component(wire));
    EXPECT_TRUE(_storage->contains_current(wire));
}


TEST_F(WireStorage, OtherNames)
{
    for( Wire& wire : _wires )
    {
        wire.add_name("test_name_1");
        wire.add_name("test_name_2");
        wire.add_name("test_name_3");

        ASSERT_NO_THROW(_storage->add_component(wire));
        EXPECT_TRUE(_storage->contains_current(wire));
    }
}


TEST_F(WireStorage, WireUpdate)
{
    Wire initial = _wires[0];

    ASSERT_NO_THROW(_storage->add_component(initial));
    EXPECT_TRUE(_storage->contains_current(initial));

    Wire modified
    (
        initial.id(),
        "new_name",
        Wire::PulledStatus::PS_NONE,
        { 0, 1 },
        { 2, 3 }
    );

    EXPECT_TRUE(_storage->contains(modified));
    EXPECT_FALSE(_storage->contains_current(modified));

    ASSERT_NO_THROW(_storage->update_component(modified));
    EXPECT_TRUE(_storage->contains_current(modified));
}


TEST_F(WireStorage, Get)
{
    Wire wire = _wires[0];
    Wire fetched;

    ASSERT_NO_THROW(_storage->add_component(wire));
    ASSERT_NO_THROW(fetched = _storage->get<Wire>(wire.id()));

    EXPECT_EQ(wire, fetched);
}


TEST_F(WireStorage, GetAll)
{
    for( const Wire& wire : _wires )
    {
        ASSERT_NO_THROW(_storage->add_component(wire));
    }

    std::vector<Wire> all_wires;
    ASSERT_NO_THROW(all_wires = _storage->get_all<Wire>());

    std::sort
    (
        all_wires.begin(),
        all_wires.end(),
        [](const Wire& a, const Wire& b)
        {
            return a.id() < b.id();
        }
    );

    EXPECT_EQ(all_wires, _wires);
}

