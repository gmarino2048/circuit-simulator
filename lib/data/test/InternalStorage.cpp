/**
 * @file InternalStorage.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Unit test file for the internal database object
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// Test Framework Includes
#include <gtest/gtest.h>

// C++ Stdlib Includes
#include <string>
#include <vector>

// Project Includes
#include <circsim/components/Wire.hpp>
#include <circsim/components/Transistor.hpp>

// File Under Test
#define private public
#define protected public
#include <circsim/data/InternalStorage.hpp>
#undef private
#undef protected

using Wire = circsim::components::Wire;
using Transistor = circsim::components::Transistor;
using InternalStorage = circsim::data::InternalStorage;


class InternalDatabaseTest : public ::testing::Test
{
protected:

    const size_t NORMAL_WIRE_COUNT = 50;
    const size_t NORMAL_TRANSISTOR_COUNT = 50;

    std::vector<Wire> _wires;
    std::vector<Transistor> _transistors;

    InternalStorage _database;

    InternalDatabaseTest() = default;
    ~InternalDatabaseTest() = default;

    void SetUp() override;

    void _verify_all_components();

};


void InternalDatabaseTest::SetUp()
{
    _wires.resize(NORMAL_WIRE_COUNT);
    _transistors.resize(NORMAL_TRANSISTOR_COUNT);

    // Initialize with disconnected wires
    for( size_t i = 0; i < NORMAL_WIRE_COUNT; i++ )
    {
        _wires[i] = Wire(i, "test", Wire::PS_NONE, { 0 }, { 0 });
    }

    // Initialize with disconnected transistors
    for( size_t i = 0; i < NORMAL_TRANSISTOR_COUNT; i++ )
    {
        _transistors[i] = Transistor(i, 0, 0, 0);
    }
}


void InternalDatabaseTest::_verify_all_components()
{
    for( size_t i = 0; i < NORMAL_WIRE_COUNT; i++ )
    {
        Wire* wire = nullptr;
        EXPECT_NO_FATAL_FAILURE(wire = _database.get_wire(i));

        ASSERT_NE(wire, nullptr);
        EXPECT_EQ(*wire, _wires[i]);
    }

    for( size_t i = 0; i < NORMAL_TRANSISTOR_COUNT; i++ )
    {
        Transistor* transistor;
        EXPECT_NO_FATAL_FAILURE(transistor = _database.get_transistor(i));

        ASSERT_NE(transistor, nullptr);
        EXPECT_EQ(*transistor, _transistors[i]);
    }
}


TEST_F(InternalDatabaseTest, DefaultConstructor)
{
    // Test no-param default construction
    EXPECT_EQ(_database._wire_instances.size(), 0);
    EXPECT_EQ(_database._transistor_instances.size(), 0);
}


TEST_F(InternalDatabaseTest, ComponentConstructor)
{
    ASSERT_NO_FATAL_FAILURE(_database = InternalStorage(_wires, _transistors));

    ASSERT_EQ(_database.wire_count(), NORMAL_WIRE_COUNT);
    ASSERT_EQ(_database.transistor_count(), NORMAL_TRANSISTOR_COUNT);

    _verify_all_components();
}


TEST_F(InternalDatabaseTest, CopyConstructor)
{
    InternalStorage db_original(_wires, _transistors);
    _database = db_original;

    // Ensure copy
    ASSERT_EQ(db_original.wire_count(), NORMAL_WIRE_COUNT);
    ASSERT_EQ(db_original.transistor_count(), NORMAL_TRANSISTOR_COUNT);

    _verify_all_components();
}


TEST_F(InternalDatabaseTest, MoveConstructor)
{
    InternalStorage db_original(_wires, _transistors);
    _database = std::move(db_original);

    _verify_all_components();
}


TEST_F(InternalDatabaseTest, AddComponentSimple)
{
    // This will trigger a recalc, so don't worry about an extra test for that
    for( const Wire& wire : _wires )
    {
        ASSERT_NO_THROW(_database.add_component(wire));
    }

    for( const Transistor& transistor : _transistors )
    {
        ASSERT_NO_THROW(_database.add_component(transistor));
    }

    _verify_all_components();
}


TEST_F(InternalDatabaseTest, FastUpdateComponent)
{
    _database = InternalStorage(_wires, _transistors);

    const size_t index = 5;
    const Wire::State new_state = Wire::PULLED_HIGH;

    {
        _database.get_wire(index)->state(new_state);
    }

    EXPECT_EQ(_database.get_wire(index)->state(), new_state);
}


TEST_F(InternalDatabaseTest, UpdateComponent)
{
    // Update Wire
    const size_t index = 5;
    const std::string new_name = "NewName";
    const std::vector<size_t> ctrl_trans = { 2, 3 };
    const std::vector<size_t> gate_trans = { 4, 5 };

    Wire new_wire(
        index,
        new_name,
        Wire::PS_HIGH,
        ctrl_trans,
        gate_trans
    );

    EXPECT_NO_THROW(_database.update_component(new_wire));

    Wire* wire_ptr = _database.get_wire(index);
    ASSERT_NE(wire_ptr, nullptr);

    EXPECT_EQ(wire_ptr->primary_name(), new_name);
    EXPECT_EQ(wire_ptr->ctrl_transistors(), ctrl_trans);
    EXPECT_EQ(wire_ptr->gate_transistors(), gate_trans);

    // Update Transistor
    size_t new_source = 5;
    size_t new_gate = 6;
    size_t new_drain = 7;

    Transistor new_transistor(
        index,
        new_gate,
        new_source,
        new_drain
    );

    _database.update_component(new_transistor);

    Transistor* trans_ptr = _database.get_transistor(index);
    ASSERT_NE(trans_ptr, nullptr);

    EXPECT_EQ(trans_ptr->source(), new_source);
    EXPECT_EQ(trans_ptr->gate(), new_gate);
    EXPECT_EQ(trans_ptr->drain(), new_drain);
}

TEST_F(InternalDatabaseTest, UpdateAddsComponent)
{
    for( const Wire& wire : _wires )
    {
        _database.update_component(wire);
    }

    for( const Transistor& transistor : _transistors )
    {
        _database.update_component(transistor);
    }

    _verify_all_components();
}
