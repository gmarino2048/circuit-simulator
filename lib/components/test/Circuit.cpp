/**
 * @file Circuit.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Unit test file for the Circuit object
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
#include <circsim/components/Circuit.hpp>
#undef private
#undef protected

using Wire = circsim::components::Wire;
using Transistor = circsim::components::Transistor;
using Circuit = circsim::components::Circuit;


class CircuitTest : public ::testing::Test
{
protected:

    const size_t NORMAL_WIRE_COUNT = 50;
    const size_t NORMAL_TRANSISTOR_COUNT = 50;

    std::vector<Wire> _wires;
    std::vector<Transistor> _transistors;

    Circuit _circuit;

    CircuitTest() = default;
    ~CircuitTest() = default;

    void SetUp() override;

    void _verify_all_components();

};


void CircuitTest::SetUp()
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


void CircuitTest::_verify_all_components()
{
    for( size_t i = 0; i < NORMAL_WIRE_COUNT; i++ )
    {
        Wire* wire = nullptr;
        EXPECT_NO_FATAL_FAILURE(wire = _circuit.get<Wire>(i));

        ASSERT_NE(wire, nullptr);
        EXPECT_EQ(*wire, _wires[i]);
    }

    for( size_t i = 0; i < NORMAL_TRANSISTOR_COUNT; i++ )
    {
        Transistor* transistor;
        EXPECT_NO_FATAL_FAILURE(transistor = _circuit.get<Transistor>(i));

        ASSERT_NE(transistor, nullptr);
        EXPECT_EQ(*transistor, _transistors[i]);
    }
}


TEST_F(CircuitTest, DefaultConstructor)
{
    // Test no-param default construction
    EXPECT_EQ(_circuit._wire_instances.size(), 0);
    EXPECT_EQ(_circuit._transistor_instances.size(), 0);
}


TEST_F(CircuitTest, ComponentInsertion)
{
    ASSERT_NO_FATAL_FAILURE(_circuit.add_all_components(_transistors));
    ASSERT_NO_FATAL_FAILURE(_circuit.add_all_components(_wires));

    ASSERT_EQ(_circuit.count<Wire>(), NORMAL_WIRE_COUNT);
    ASSERT_EQ(_circuit.count<Transistor>(), NORMAL_TRANSISTOR_COUNT);

    _verify_all_components();
}


TEST_F(CircuitTest, CopyConstructor)
{
    Circuit db_original;
    ASSERT_NO_FATAL_FAILURE(db_original.add_all_components(_transistors));
    ASSERT_NO_FATAL_FAILURE(db_original.add_all_components(_wires));

    _circuit = db_original;

    // Ensure copy
    ASSERT_EQ(db_original.count<Wire>(), NORMAL_WIRE_COUNT);
    ASSERT_EQ(db_original.count<Transistor>(), NORMAL_TRANSISTOR_COUNT);

    _verify_all_components();
}


TEST_F(CircuitTest, MoveConstructor)
{
    Circuit db_original;
    ASSERT_NO_FATAL_FAILURE(db_original.add_all_components(_transistors));
    ASSERT_NO_FATAL_FAILURE(db_original.add_all_components(_wires));

    _circuit = std::move(db_original);

    _verify_all_components();
}


TEST_F(CircuitTest, AddComponentSimple)
{
    // This will trigger a recalc, so don't worry about an extra test for that
    for( const Wire& wire : _wires )
    {
        ASSERT_NO_THROW(_circuit.add_component(wire));
    }

    for( const Transistor& transistor : _transistors )
    {
        ASSERT_NO_THROW(_circuit.add_component(transistor));
    }

    _verify_all_components();
}


TEST_F(CircuitTest, FastUpdateComponent)
{
    ASSERT_NO_FATAL_FAILURE(_circuit.add_all_components(_transistors));
    ASSERT_NO_FATAL_FAILURE(_circuit.add_all_components(_wires));

    const size_t index = 5;
    const Wire::State new_state = Wire::PULLED_HIGH;

    {
        _circuit.get<Wire>(index)->state(new_state);
    }

    EXPECT_EQ(_circuit.get<Wire>(index)->state(), new_state);
}


TEST_F(CircuitTest, UpdateComponent)
{
    // Update Wire
    const size_t index = 5;
    const std::string new_name = "NewName";
    const std::vector<uint64_t> ctrl_trans = { 2, 3 };
    const std::vector<uint64_t> gate_trans = { 4, 5 };

    Wire new_wire(
        index,
        new_name,
        Wire::PS_HIGH,
        ctrl_trans,
        gate_trans
    );

    EXPECT_NO_THROW(_circuit.update_component(new_wire));

    Wire* wire_ptr = _circuit.get<Wire>(index);
    ASSERT_NE(wire_ptr, nullptr);

    EXPECT_EQ(wire_ptr->primary_name(), new_name);
    EXPECT_EQ(wire_ptr->ctrl_transistors(), ctrl_trans);
    EXPECT_EQ(wire_ptr->gate_transistors(), gate_trans);

    // Update Transistor
    uint64_t new_source = 5;
    uint64_t new_gate = 6;
    uint64_t new_drain = 7;

    Transistor new_transistor(
        index,
        new_gate,
        new_source,
        new_drain
    );

    _circuit.update_component(new_transistor);

    Transistor* trans_ptr = _circuit.get<Transistor>(index);
    ASSERT_NE(trans_ptr, nullptr);

    EXPECT_EQ(trans_ptr->source(), new_source);
    EXPECT_EQ(trans_ptr->gate(), new_gate);
    EXPECT_EQ(trans_ptr->drain(), new_drain);
}

TEST_F(CircuitTest, UpdateAddsComponent)
{
    for( const Wire& wire : _wires )
    {
        _circuit.update_component(wire);
    }

    for( const Transistor& transistor : _transistors )
    {
        _circuit.update_component(transistor);
    }

    _verify_all_components();
}
