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
#include <cstdint>
#include <limits>
#include <vector>

// Test Framework Includes
#include <gtest/gtest.h>

// Project Includes
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
                std::numeric_limits<uint64_t>::max(),
                "wire2",
                Wire::PulledStatus::PS_LOW,
                {},
                { 1, 2, 3, 4, 5, 6 }
            ),
            Wire
            (
                84,
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


TEST_F(WireStorage, ElementFidelity)
{
    for( const Wire& wire : _wires )
    {
        ASSERT_NO_THROW(_storage->add_component(wire));
        EXPECT_TRUE(_storage->contains_current(wire));
    }
}
