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

    Register *_test_register_8 = nullptr;
    Register *_test_register_16 = nullptr;
    Register *_test_register_32 = nullptr;
    Register *_test_register_64 = nullptr;

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
    std::array<Register**, 4> registers = {
        &_test_register_8,
        &_test_register_16,
        &_test_register_32,
        &_test_register_64
    };

    for( size_t i = 0; i < registers.size(); i++ )
    {
        Register** register_ptr_ref = registers[i];
        size_t register_size = sizes[i];

        std::vector<uint64_t> wire_ids;
        wire_ids.reserve(register_size);

        for( size_t j = current; j < current + register_size; j++ )
        {
            wire_ids.push_back(wires[j].id());
        }

        current += register_size;

        *register_ptr_ref = new Register
        (
            i,
            std::to_string(register_size),
            wire_ids,
            *_circuit
        );
    }
}

void RegisterTest::TearDown()
{
    delete _test_register_64;
    delete _test_register_32;
    delete _test_register_16;
    delete _test_register_8;

    delete _circuit;
}

TEST_F(RegisterTest, TestRegisterZero)
{
    std::array<Register*, 4> registers = {
        _test_register_8,
        _test_register_16,
        _test_register_32,
        _test_register_64
    };

    for( Register* reg : registers )
    {
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
    _test_register_8->value_unsigned((uint64_t) 9);
    _test_register_16->value_unsigned((uint64_t) 17);
    _test_register_32->value_unsigned((uint64_t) 33);
    _test_register_64->value_unsigned((uint64_t) 65);

    // Check the wires manually to make sure they were all changed
    std::array<Register*, 4> registers = {
        _test_register_8,
        _test_register_16,
        _test_register_32,
        _test_register_64
    };
    for( size_t i = 0; i < registers.size(); i++ )
    {
        Register* reg = registers[i];
        std::vector<uint64_t> wire_ids = reg->wire_ids();

        size_t expected_high = i + 3;
        EXPECT_TRUE(_circuit->get<Wire>(wire_ids[0])->high());
        EXPECT_TRUE(_circuit->get<Wire>(wire_ids[expected_high])->high());
    }

    // Now test with decimal comparison
    EXPECT_EQ(_test_register_8->value_unsigned<uint64_t>(), 9);
    EXPECT_EQ(_test_register_16->value_unsigned<uint64_t>(), 17);
    EXPECT_EQ(_test_register_32->value_unsigned<uint64_t>(), 33);
    EXPECT_EQ(_test_register_64->value_unsigned<uint64_t>(), 65);
}
