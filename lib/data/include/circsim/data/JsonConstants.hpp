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

const char* TRANSISTOR_VALUE_ID = "id";
const char* TRANSISTOR_VALUE_NAME = "name";
const char* TRANSISTOR_VALUE_TYPE = "type";
const char* TRANSISTOR_VALUE_GATE = "gate_wire";
const char* TRANSISTOR_VALUE_SOURCE = "source_wire";
const char* TRANSISTOR_VALUE_DRAIN = "drain_wire";

}

#endif
