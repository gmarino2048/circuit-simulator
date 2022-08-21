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
    return circsim::data::JsonParser::parse(NAND_DEF);
}
