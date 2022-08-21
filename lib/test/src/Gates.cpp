/**
 * @file Gates.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief File for easily making logic gates for the test framework
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
// (none)

// Test Framework Includes
#include <Gates.hpp>

// Project Includes
#include <circsim/components/Circuit.hpp>
#include <circsim/components/Wire.hpp>
#include <circsim/components/Transistor.hpp>
#include <circsim/data/JsonParser.hpp>

using Wire = circsim::components::Wire;
using Transistor = circsim::components::Transistor;

circsim::components::Circuit create_nor()
{
    return circsim::data::JsonParser::parse(NOR_DEF);
}

circsim::components::Circuit create_nand()
{
    using Wire = circsim::components::Wire;
    using Transistor = circsim::components::Transistor;

    Wire gnd
    (
        0,
        Wire::SW_GND,
        { 2 },          // Connected to drain of first control transistor
        { }
    );

    Wire in_a
    (
        1,
        "IN_A",
        Wire::PS_NONE,
        { },            // No control transistors
        { 1 }           // Controls first transistor
    );

    Wire in_b
    (
        2,
        "IN_B",
        Wire::PS_NONE,
        { },            // No control transistors
        { 2 }           // Controls second transistor
    );


    Wire connector
    (
        3,
        "connector",
        Wire::PS_NONE,
        { 1, 2 },       // Bridges first and second transistors
        { }
    );

    Wire out
    (
        4,
        "OUT",
        Wire::PS_HIGH,
        { 1 },          // Connected to source of second control transistor
        { }
    );


    Transistor trans_a
    (
        1,
        in_a.id(),
        out.id(),
        connector.id()
    );

    Transistor trans_b
    (
        2,
        in_b.id(),
        connector.id(),
        gnd.id()
    );

    circsim::components::Circuit circuit;
    circuit.add_all_components<Wire>({ gnd, in_a, in_b, connector, out });
    circuit.add_all_components<Transistor>({ trans_a, trans_b });

    return circuit;
}
