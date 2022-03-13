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
        "CREATE TABLE wires IF NOT EXISTS (" +
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
