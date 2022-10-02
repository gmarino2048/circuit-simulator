/**
 * @file Register.cpp
 * @author Guy Marino (gxm262@case.edu)
 * @brief Tests the database implementation of the register object
 * @version 0.1
 * @date 2022-10-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <utility>

// Test Framework Includes
#include <gtest/gtest.h>

// Project Includes
#include <circsim/common/StateError.hpp>
#include <circsim/components/Circuit.hpp>
#include <circsim/components/Register.hpp>
#include <circsim/data/ExternalStorage.hpp>

using namespace circsim::components;
using namespace circsim::data;

class RegisterStorage : public ::testing::Test
{
protected:

    ExternalStorage *_storage;
    std::vector<Register> _registers;

    void SetUp() override;

    void TearDown() override;

};


void RegisterStorage::SetUp()
{
    _storage = new ExternalStorage();

    _registers =
    {
        Register
        (
            0,
            "X",
            { 7, 6, 5, 4, 3, 2, 1, 0 }
        ),
        Register
        (
            1,
            "Y",
            {}
        ),
        Register
        (
            2,
            "",
            { 800, 45, 32, 198 }
        )
    };
}


void RegisterStorage::TearDown()
{
    delete _storage;
}


TEST_F(RegisterStorage, Count)
{
    for( const Register& reg : _registers )
    {
        ASSERT_NO_THROW(_storage->add_component(reg));
    }

    EXPECT_EQ(_storage->count<Register>(), _registers.size());
}


TEST_F(RegisterStorage, ElementFidelity)
{
    Circuit circuit("test");

    Register reg = _registers[0];
    reg.set_circuit(circuit);

    ASSERT_NO_THROW(_storage->add_component(reg));

    reg.clear_circuit();
    EXPECT_TRUE(_storage->contains_current(reg));

    Register new_reg(0, "", {});
    ASSERT_NO_THROW(new_reg = _storage->get<Register>(0));

    EXPECT_FALSE(new_reg.has_circuit());
    EXPECT_EQ(reg, new_reg);
}


TEST_F(RegisterStorage, DuplicateAddition)
{
    Register reg = _registers[0];

    ASSERT_NO_THROW(_storage->add_component(reg));
    EXPECT_THROW(_storage->add_component(reg), circsim::common::StateError);
}