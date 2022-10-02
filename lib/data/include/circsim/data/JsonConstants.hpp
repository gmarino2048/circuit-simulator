/**
 * @file JsonConstants.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief This file contains constant values required for
 *        conversion to and from JSON objects.
 * @version 0.1
 * @date 2022-08-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_DATA_JSONCONSTANTS_HPP
#define __CIRCSIM_DATA_JSONCONSTANTS_HPP

namespace circsim::data
{

// Register Definitions

/// @brief Register ID field
const char* REGISTER_VALUE_ID = "id";

/// @brief Register Name field
const char* REGISTER_VALUE_NAME = "name";

/// @brief Register Wire IDs field
const char* REGISTER_VALUE_WIREID = "wire_ids";


// Transistor Definitions

/// @brief Transistor type NMOS literal
const char* TRANSISTOR_TYPE_NMOS = "NMOS";

/// @brief Transistor type PMOS literal
const char* TRANSISTOR_TYPE_PMOS = "PMOS";


/// @brief Transistor ID field
const char* TRANSISTOR_VALUE_ID = "id";

/// @brief Transistor Name field
const char* TRANSISTOR_VALUE_NAME = "name";

/// @brief Transistor Type field
const char* TRANSISTOR_VALUE_TYPE = "type";

/// @brief Transistor Gate Wire field
const char* TRANSISTOR_VALUE_GATE = "gate_wire";

/// @brief Transistor Source Wire field
const char* TRANSISTOR_VALUE_SOURCE = "source_wire";

/// @brief Transistor Drain Wire field
const char* TRANSISTOR_VALUE_DRAIN = "drain_wire";


// Wire Definitions

/// @brief Wire PulledStatus HIGH literal
const char* WIRE_PULLED_HIGH = "HIGH";

/// @brief Wire PulledStatus LOW literal
const char* WIRE_PULLED_LOW = "LOW";

/// @brief Wire PulledStatus NONE literal
const char* WIRE_PULLED_NONE = "NONE";


/// @brief Wire SpecialWireType VCC literal
const char* WIRE_SPECIAL_VCC = "VCC";

/// @brief Wire SpecialWireType GND literal
const char* WIRE_SPECIAL_GND = "GND";

/// @brief Wire SpecialWireType NONE literal
const char* WIRE_SPECIAL_NONE = "NONE";


/// @brief Wire ID field
const char* WIRE_VALUE_ID = "id";

/// @brief Wire Name field
const char* WIRE_VALUE_NAME = "name";

/// @brief Wire Alternate Names field
const char* WIRE_VALUE_ALT = "alternate_names";

/// @brief Wire PulledStatus field
const char* WIRE_VALUE_PULLED = "pulled";

/// @brief Wire SpecialWireType field
const char* WIRE_VALUE_SPECIAL = "special";

/// @brief Wire Control Transistor Field
const char* WIRE_VALUE_CTRL = "ctrl_transistors";

/// @brief Wire Gate Transistor Field
const char* WIRE_VALUE_GATE = "gate_transistors";


// Circuit Definitions

/// @brief Circuit Name field
const char* CIRCUIT_VALUE_NAME = "name";

/// @brief Circuit Registers field
const char* CIRCUIT_VALUE_REGISTERS = "registers";

/// @brief Circuit Transistors field
const char* CIRCUIT_VALUE_TRANSISTORS = "transistors";

/// @brief Circuit Wires field
const char* CIRCUIT_VALUE_WIRES = "wires";

}

#endif
