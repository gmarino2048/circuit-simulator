/**
 * @file WireDatabaseObjects.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Tests the wire database object
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// Test framework include
#include <gtest/gtest.h>

// C++ Stdlib includes
#include <string.h>

// Project includes
#include <circsim/components/Wire.hpp>
#include <circsim/data/objects/WireDatabaseObject.hpp>

using namespace circsim::components;
using namespace circsim::data;
using namespace circsim::data::objects;

TEST(WireDatabaseObject, TestTableCreation)
{
    std::string table_creation_string = (std::string)
        "CREATE TABLE IF NOT EXISTS wires (" +
        "id INTEGER PRIMARY KEY NOT NULL, " +
        "special INTEGER NOT NULL, " +
        "primary_name TEXT NOT NULL, " +
        "other_names TEXT NOT NULL, " +
        "pulled INTEGER NOT NULL, " +
        "trans_ctrl_ids TEXT NOT NULL, " +
        "trans_gate_ids TEXT NOT NULL);";

    WireDatabaseObject object;
    std::string result = object.create_table();

    ASSERT_EQ(table_creation_string, result);
}


TEST(WireDatabaseObject, TestWireInsertion)
{
    Wire wire_obj
    (
        48,
        "test_wire",
        Wire::PulledStatus::PS_HIGH,
        { 3, 4, 5 },
        { 6, 7, 8 }
    );

    std::string wire_insertion_string = (std::string)
        "INSERT INTO wires " +
        "( id, special, primary_name, other_names, pulled, " +
        "trans_ctrl_ids, trans_gate_ids ) VALUES " +
        "( 48, 0, \"test_wire\", \"\", 1, \"3,4,5\", \"6,7,8\" );";

    WireDatabaseObject object(wire_obj);
    std::string result = object.insert_item();

    ASSERT_EQ(wire_insertion_string, result);
}

TEST(WireDatabaseObject, TestMultipleWireInsertion)
{
    Wire wire
    (
        48,
        "test_wire",
        Wire::PulledStatus::PS_HIGH,
        { 3, 4, 5 },
        { 6, 7, 8 }
    );

    Wire wire2
    (
        24,
        "test_wire2",
        Wire::PulledStatus::PS_LOW,
        { 1, 2, 3 },
        { 10, 11, 12 }
    );

    Wire wire3
    (
        0,
        Wire::SpecialWireType::GND,
        { 1, 3, 5 },
        { 2, 4, 6 }
    );

    wire2.add_name("other_name1");
    wire2.add_name("other_name2");

    std::string wire_insertion_string = (std::string)
        "INSERT INTO wires " +
        "( id, special, primary_name, other_names, pulled, " +
        "trans_ctrl_ids, trans_gate_ids ) VALUES " +

        "( 48, 0, \"test_wire\", \"\", 1, \"3,4,5\", \"6,7,8\" ), " +

        "( 24, 0, \"test_wire2\", \"'other_name1','other_name2'\", 2," +
        " \"1,2,3\", \"10,11,12\" ), " +
        
        "( 0, 1, \"GND\", \"\", 0, \"1,3,5\", \"2,4,6\" );";

    WireDatabaseObject db_objects[3] = { wire, wire2, wire3 };
    std::vector<DatabaseObject*> objects;

    for ( WireDatabaseObject &object : db_objects )
    {
        objects.push_back(static_cast<DatabaseObject*>(&object));
    }

    std::string result = db_objects[0].insert_all(objects);

    ASSERT_EQ(wire_insertion_string, result);
}
