#ifndef __CIRCSIM_TEST_COMMON_NAND_HPP
#define __CIRCSIM_TEST_COMMON_NAND_HPP

#include <circsim/components/Wire.hpp>
#include <circsim/components/Transistor.hpp>

#include <circsim/data/InternalDatabase.hpp>


circsim::data::InternalDatabase create_nand()
{
    using Wire = circsim::components::Wire;
    using Transistor = circsim::components::Transistor;

    Wire gnd
    (
        0,
        Wire::GND,
        { 2 },          // Connected to drain of first control transistor
        { }
    );

    Wire out
    (
        4,
        "OUT",
        Wire::PS_HIGH,
        { 1 },          // Connected to gate of second control transistor
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

    return circsim::data::InternalDatabase
    (
        { gnd, out, connector, in_a, in_b },
        { trans_a, trans_b }
    );
}


#endif