
#include "Gates.hpp"

#include <circsim/components/Wire.hpp>
#include <circsim/components/Transistor.hpp>

#include <circsim/data/InternalDatabase.hpp>

using Wire = circsim::components::Wire;
using Transistor = circsim::components::Transistor;

circsim::data::InternalDatabase create_nor()
{
    

    Wire gnd
    (
        0,
        Wire::GND,
        { 1, 2 },       // Connected to drain of both transistors
        { }
    );

    Wire in_a
    (
        1,
        "IN_A",
        Wire::PS_NONE,  // Not connected to pullup/pulldown
        { },
        { 1 }           // Connected to gate of transistor A
    );

    Wire in_b
    (
        2,
        "IN_B",
        Wire::PS_NONE,  // Not connected to pullup/pulldown
        { },
        { 2 }           // Connected to gate of transistor B
    );

    Wire out
    (
        3,
        "OUT",
        Wire::PS_HIGH,  // Connected to a pullup
        { 1, 2 },       // Connected to source of both transistors
        { }
    );

    Transistor trans_a
    (
        1,
        in_a.id(),
        out.id(),
        gnd.id()
    );

    Transistor trans_b
    (
        2,
        in_b.id(),
        out.id(),
        gnd.id()
    );

    return circsim::data::InternalDatabase
    (
        { out, in_a, in_b, gnd },
        { trans_a, trans_b }
    );
}

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

    return circsim::data::InternalDatabase
    (
        { gnd, out, connector, in_a, in_b },
        { trans_a, trans_b }
    );
}
