/**
 * @file Register.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Tests for the register component.
 * @version 0.1
 * @date 2022-09-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <array>
#include <cinttypes>
#include <limits>
#include <vector>

// Library Includes
#include <gtest/gtest.h>

// Project Includes
#include <circsim/components/Circuit.hpp>
#include <circsim/components/Register.hpp>
#include <circsim/components/Wire.hpp>

using namespace circsim::common;
using namespace circsim::components;

class RegisterTest : public ::testing::Test
{
protected:

    Circuit *_circuit;

    RegisterTest() = default;
    ~RegisterTest() = default;

    void SetUp() override;
    void TearDown() override;

};

void RegisterTest::SetUp()
{
    std::array<size_t, 4> sizes = { 8, 16, 32, 64 };

    size_t wire_count = 0;
    for( size_t size : sizes )
    {
        wire_count += size;
    }

    // Create the circuit object
    _circuit = new Circuit("register_test");

    // Create the wires for the circuit object
    std::vector<Wire> wires;
    wires.reserve(wire_count);

    for( size_t i = 0; i < wire_count; i++ )
    {
        wires.emplace_back
        (
            i,
            std::to_string(i),
            Wire::PS_NONE,
            std::vector<uint64_t>(),
            std::vector<uint64_t>()
        );
    }

    _circuit->add_all_components<Wire>(wires);

    size_t current = 0;
    std::vector<Register> registers;

    for( size_t i = 0; i < sizes.size(); i++ )
    {
        size_t register_size = sizes[i];

        std::vector<uint64_t> wire_ids;
        wire_ids.reserve(register_size);

        for( size_t j = current; j < current + register_size; j++ )
        {
            wire_ids.push_back(wires[j].id());
        }

        current += register_size;

        registers.emplace_back
        (
            i,
            std::to_string(register_size),
            wire_ids,
            *_circuit
        );
    }

    _circuit->add_all_components<Register>(registers);
}

void RegisterTest::TearDown()
{
    delete _circuit;
}

TEST_F(RegisterTest, TestRegisterZero)
{
    for( size_t i = 0; i < _circuit->count<Register>(); i++ )
    {
        Register* reg = _circuit->get<Register>(i);
        auto check_wires_zero = [this,reg]()
        {
            for( uint64_t wire_id : reg->wire_ids() )
            {
                Wire* instance = _circuit->get<Wire>(wire_id);
                EXPECT_TRUE(instance->low());
            }
        };

        reg->value_unsigned<uint64_t>(0);
        EXPECT_EQ(reg->value_unsigned<uint64_t>(), 0);
        check_wires_zero();

        reg->value_signed<int64_t>(0);
        EXPECT_EQ(reg->value_signed<int64_t>(), 0);
        check_wires_zero();
    }
}

TEST_F(RegisterTest, TestRegistersUnsigned)
{
    _circuit->get<Register>(0)->value_unsigned((uint64_t) 9);
    _circuit->get<Register>(1)->value_unsigned((uint64_t) 17);
    _circuit->get<Register>(2)->value_unsigned((uint64_t) 33);
    _circuit->get<Register>(3)->value_unsigned((uint64_t) 65);

    // Check the wires manually to make sure they were all changed
    for( size_t i = 0; i < _circuit->count<Register>(); i++ )
    {
        Register* reg = _circuit->get<Register>(i);
        std::vector<uint64_t> wire_ids = reg->wire_ids();

        size_t expected_high = i + 3;
        EXPECT_TRUE(_circuit->get<Wire>(wire_ids[0])->high());
        EXPECT_TRUE(_circuit->get<Wire>(wire_ids[expected_high])->high());
    }

    // Now test with decimal comparison
    EXPECT_EQ(_circuit->get<Register>(0)->value_unsigned<uint64_t>(), 9);
    EXPECT_EQ(_circuit->get<Register>(1)->value_unsigned<uint64_t>(), 17);
    EXPECT_EQ(_circuit->get<Register>(2)->value_unsigned<uint64_t>(), 33);
    EXPECT_EQ(_circuit->get<Register>(3)->value_unsigned<uint64_t>(), 65);
}


TEST_F(RegisterTest, TestRegistersSigned)
{
    _circuit->get<Register>(0)->value_signed(-9);
    _circuit->get<Register>(1)->value_signed(-17);
    _circuit->get<Register>(2)->value_signed(-33);
    _circuit->get<Register>(3)->value_signed(-65);

    EXPECT_EQ(_circuit->get<Register>(0)->value_signed<int64_t>(), -9);
    EXPECT_EQ(_circuit->get<Register>(1)->value_signed<int64_t>(), -17);
    EXPECT_EQ(_circuit->get<Register>(2)->value_signed<int64_t>(), -33);
    EXPECT_EQ(_circuit->get<Register>(3)->value_signed<int64_t>(), -65);
}

TEST_F(RegisterTest, TestRegisterUnsignedMax)
{
    _circuit->get<Register>(0)->value_unsigned(std::numeric_limits<uint8_t>::max());
    _circuit->get<Register>(1)->value_unsigned(std::numeric_limits<uint16_t>::max());
    _circuit->get<Register>(2)->value_unsigned(std::numeric_limits<uint32_t>::max());
    _circuit->get<Register>(3)->value_unsigned(std::numeric_limits<uint64_t>::max());

    EXPECT_EQ
    (
        _circuit->get<Register>(0)->value_unsigned<uint8_t>(),
        std::numeric_limits<uint8_t>::max()
    );

    EXPECT_EQ
    (
        _circuit->get<Register>(1)->value_unsigned<uint16_t>(),
        std::numeric_limits<uint16_t>::max()
    );

    EXPECT_EQ
    (
        _circuit->get<Register>(2)->value_unsigned<uint32_t>(),
        std::numeric_limits<uint32_t>::max()
    );

    EXPECT_EQ
    (
        _circuit->get<Register>(3)->value_unsigned<uint64_t>(),
        std::numeric_limits<uint64_t>::max()
    );
}
