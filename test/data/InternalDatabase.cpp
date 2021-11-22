
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

    void _verify_all_components();

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
    ASSERT_NO_FATAL_FAILURE(_database = InternalDatabase(_wires, _transistors));

    ASSERT_EQ(_database.wire_count(), NORMAL_WIRE_COUNT);
    ASSERT_EQ(_database.transistor_count(), NORMAL_TRANSISTOR_COUNT);

    _verify_all_components();
}


TEST_F(InternalDatabaseTest, CopyConstructor)
{
    InternalDatabase db_original(_wires, _transistors);
    _database = db_original;

    // Ensure copy
    ASSERT_EQ(db_original.wire_count(), NORMAL_WIRE_COUNT);
    ASSERT_EQ(db_original.transistor_count(), NORMAL_TRANSISTOR_COUNT);

    _verify_all_components();
}


TEST_F(InternalDatabaseTest, MoveConstructor)
{
    InternalDatabase db_original(_wires, _transistors);
}


TEST_F(InternalDatabaseTest, AddComponentSimple)
{
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
