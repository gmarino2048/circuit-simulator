/**
 * @file CircuitState.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Unit testing for the DB implementation of the CircuitState object.
 * @version 0.1
 * @date 2022-08-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <limits>
#include <vector>

// Test Framework Includes
#include <gtest/gtest.h>

// Project Includes
#include <circsim/common/StateError.hpp>
#include <circsim/components/CircuitState.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>
#include <circsim/data/ExternalStorage.hpp>

using namespace circsim::components;
using namespace circsim::data;


class CircuitStateStorage : public ::testing::Test
{
protected:

    ExternalStorage* _storage;
    std::vector<CircuitState> _snapshots;

    void SetUp()
    {
        _storage = new ExternalStorage();

        std::vector<uint64_t> t_ids = { 0, 4, 44, 444, std::numeric_limits<uint64_t>::max() };
        std::vector<Transistor::State> t_states = { Transistor::ON, Transistor::OFF, Transistor::ON, Transistor::OFF, Transistor::ON };

        std::vector<uint64_t> w_ids = { 0, 9, 99, 999, 9999, 99999, std::numeric_limits<uint64_t>::max() };
        std::vector<Wire::State> w_states =
        {
            Wire::GROUNDED,
            Wire::PULLED_LOW,
            Wire::FLOATING_LOW,
            Wire::FLOATING,
            Wire::FLOATING_HIGH,
            Wire::PULLED_HIGH,
            Wire::HIGH
        };

        CircuitState snapshot1(0xCAFE);
        snapshot1.update_multiple_states_manual<Transistor>(t_ids, t_states);
        snapshot1.update_multiple_states_manual<Wire>(w_ids, w_states);

        _snapshots.push_back(snapshot1);
    }


    void TearDown()
    {
        delete _storage;
    }

};


TEST_F(CircuitStateStorage, FidelityTest)
{
    CircuitState snapshot = _snapshots[0];

    ASSERT_NO_THROW(_storage->add_component(snapshot));
    ASSERT_TRUE(_storage->contains_current(snapshot));
}


TEST_F(CircuitStateStorage, ContainsTest)
{
    CircuitState snapshot = _snapshots[0];

    ASSERT_NO_THROW(_storage->add_component(snapshot));
    ASSERT_TRUE(_storage->contains(snapshot));

    snapshot.update_object_state_manual<Wire>(0, Wire::PULLED_HIGH);

    ASSERT_TRUE(_storage->contains(snapshot));
}


TEST_F(CircuitStateStorage, UpdateTest)
{
    CircuitState snapshot = _snapshots[0];

    ASSERT_NO_THROW(_storage->add_component(snapshot));
    ASSERT_TRUE(_storage->contains_current(snapshot));

    snapshot.update_object_state_manual<Transistor>(0, Transistor::OFF);

    ASSERT_FALSE(_storage->contains_current(snapshot));
    ASSERT_NO_THROW(_storage->update_component(snapshot));
    ASSERT_TRUE(_storage->contains_current(snapshot));

    snapshot.update_object_state_manual<Wire>(0, Wire::PULLED_HIGH);

    ASSERT_FALSE(_storage->contains_current(snapshot));
    ASSERT_NO_THROW(_storage->update_component(snapshot));
    ASSERT_TRUE(_storage->contains_current(snapshot));
}


TEST_F(CircuitStateStorage, DoubleInsertion)
{
    CircuitState snapshot = _snapshots[0];

    ASSERT_NO_THROW(_storage->add_component(snapshot));
    ASSERT_TRUE(_storage->contains_current(snapshot));

    EXPECT_THROW(_storage->add_component(snapshot), circsim::common::StateError);
}


TEST_F(CircuitStateStorage, UpdateInsertion)
{
    CircuitState snapshot = _snapshots[0];

    ASSERT_NO_THROW(_storage->update_component(snapshot));
    ASSERT_TRUE(_storage->contains_current(snapshot));
}
