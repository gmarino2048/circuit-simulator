/**
 * @file TransistorDatabaseObject.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Unit test file for the transistor database object
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
#include <string>
#include <vector>

// Library Includes
#include <gtest/gtest.h>

// Project Includes
#include <circsim/data/objects/TransistorDatabaseObject.hpp>

using namespace circsim::components;
using namespace circsim::data;
using namespace circsim::data::objects;

TEST(TransistorDatabaseObject, TestTableCreation)
{
    std::string table_creation_string = (std::string)
        "CREATE TABLE IF NOT EXISTS transistors (" +
        "id INTEGER PRIMARY KEY NOT NULL, " +
        "name TEXT NOT NULL, " +
        "type INTEGER NOT NULL, " +
        "gate_id INTEGER NOT NULL, " +
        "src_id INTEGER NOT NULL, " +
        "drain_id INTEGER NOT NULL);";

    TransistorDatabaseObject object;
    std::string result = object.create_table();

    ASSERT_EQ(table_creation_string, result);
}

TEST(TransistorDatabaseObject, TestTransistorInsertion)
{
    Transistor trans_obj
    (
        123,
        456,
        789,
        101112
    );

    std::string transistor_insertion_string = (std::string)
        "INSERT INTO transistors " +
        "( id, name, type, gate_id, src_id, drain_id ) VALUES " +
        "( 123, \"\", 0, 456, 789, 101112 );";

    TransistorDatabaseObject object(trans_obj);
    std::string result = object.insert_item();

    ASSERT_EQ(transistor_insertion_string, result);
}

TEST(TransistorDatabaseObject, TestMultipleTransistorInsertion)
{
    Transistor trans
    (
        48,
        "Named",
        100,
        110,
        120
    );

    Transistor trans2
    (
        24,
        "Named2",
        230,
        240,
        250,
        Transistor::Type::PMOS
    );

    Transistor trans3
    (
        0,
        360,
        370,
        380
    );

    std::string transistor_insertion_string = (std::string)
        "INSERT INTO transistors " +
        "( id, name, type, gate_id, src_id, drain_id ) VALUES " +

        "( 48, \"Named\", 0, 100, 110, 120 ), " +

        "( 24, \"Named2\", 1, 230, 240, 250 ), " +

        "( 0, \"\", 0, 360, 370, 380 );";

    TransistorDatabaseObject db_objects[3] = { trans, trans2, trans3 };
    std::vector<DatabaseObject*> objects;

    for ( TransistorDatabaseObject &object : db_objects )
    {
        objects.push_back(static_cast<DatabaseObject*>(&object));
    }

    std::string result = db_objects[0].insert_all(objects);

    ASSERT_EQ(transistor_insertion_string, result);
}
