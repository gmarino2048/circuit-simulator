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

// Transistor Definitions

const char* TRANSISTOR_TYPE_NMOS = "NMOS";
const char* TRANSISTOR_TYPE_PMOS = "PMOS";

const char* TRANSISTOR_VALUE_ID = "id";
const char* TRANSISTOR_VALUE_NAME = "name";
const char* TRANSISTOR_VALUE_TYPE = "type";
const char* TRANSISTOR_VALUE_GATE = "gate_wire";
const char* TRANSISTOR_VALUE_SOURCE = "source_wire";
const char* TRANSISTOR_VALUE_DRAIN = "drain_wire";


// Wire Definitions

const char* WIRE_PULLED_HIGH = "HIGH";
const char* WIRE_PULLED_LOW = "LOW";
const char* WIRE_PULLED_NONE = "NONE";

const char* WIRE_SPECIAL_VCC = "VCC";
const char* WIRE_SPECIAL_GND = "GND";
const char* WIRE_SPECIAL_NONE = "NONE";

const char* WIRE_VALUE_ID = "id";
const char* WIRE_VALUE_NAME = "name";
const char* WIRE_VALUE_ALT = "alternate_names";
const char* WIRE_VALUE_PULLED = "pulled";
const char* WIRE_VALUE_SPECIAL = "special";
const char* WIRE_VALUE_CTRL = "ctrl_transistors";
const char* WIRE_VALUE_GATE = "gate_transistors";

}

#endif
