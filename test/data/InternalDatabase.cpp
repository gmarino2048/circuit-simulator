
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <circsim/components/Wire.hpp>
#include <circsim/components/Transistor.hpp>

#define private public
#define protected public
#include <circsim/data/InternalDatabase.hpp>
#undef private
#undef protected

using Wire = circsim::components::Wire;
using Transistor = circsim::components::Transistor;
using InternalDatabase = circsim::data::InternalDatabase;


class InternalDatabaseTest : public ::testing::Test
{
protected:

    const size_t NORMAL_WIRE_COUNT = 50;
    const size_t NORMAL_TRANSISTOR_COUNT = 50;

    std::vector<Wire> _wires;
    std::vector<Transistor> _transistors;

    InternalDatabase _database;

    InternalDatabaseTest() = default;
    ~InternalDatabaseTest() = default;

    void SetUp() override;

};


void InternalDatabaseTest::SetUp()
{
    _wires.resize(NORMAL_WIRE_COUNT);
    _transistors.resize(NORMAL_TRANSISTOR_COUNT);

    // Initialize with disconnected wires
    for( size_t i = 0; i < NORMAL_WIRE_COUNT; i++ )
    {
        _wires[i] = Wire(i, "test", false, { 0 }, { 0 });
    }

    // Initialize with disconnected transistors
    for( size_t i = 0; i < NORMAL_TRANSISTOR_COUNT; i++ )
    {
        _transistors[i] = Transistor(i, 0, 0, 0);
    }
}


TEST_F(InternalDatabaseTest, DefaultConstructor)
{
    // Test no-param default construction
    EXPECT_EQ(_database._wire_instances.size(), 0);
    EXPECT_EQ(_database._transistor_instances.size(), 0);

    // Test parametric default construction
    _database = InternalDatabase(NORMAL_WIRE_COUNT, NORMAL_TRANSISTOR_COUNT);

    EXPECT_EQ(_database.wire_count(), NORMAL_WIRE_COUNT);
    EXPECT_EQ(_database.transistor_count(), NORMAL_TRANSISTOR_COUNT);

    for( const Wire& wire : _database._wire_instances )
    {
        // Expect wires created by constructor are invalid
        EXPECT_EQ(wire.id(), -1);
    }

    for( const Transistor& transistor : _database._transistor_instances )
    {
        // Expect transistors created by constructor are invalid
        EXPECT_EQ(transistor.id(), -1);
    }
}


TEST_F(InternalDatabaseTest, ComponentConstructor)
{
    ASSERT_NO_FATAL_FAILURE
    (
        _database = InternalDatabase(_wires, _transistors);
    );

    ASSERT_EQ(_database.wire_count(), NORMAL_WIRE_COUNT);
    ASSERT_EQ(_database.transistor_count(), NORMAL_TRANSISTOR_COUNT);

    for( size_t i = 0; i < NORMAL_WIRE_COUNT; i++ )
    {
        EXPECT_NO_FATAL_FAILURE(_database.get_wire(i));
    }

    for( size_t i = 0; i < NORMAL_TRANSISTOR_COUNT; i++ )
    {
        EXPECT_NO_FATAL_FAILURE(_database.get_transistor(i));
    }
}
