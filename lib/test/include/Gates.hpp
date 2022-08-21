/**
 * @file Gates.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Test framework file for importing logic gates
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_TEST_COMMON_GATES_HPP
#define __CIRCSIM_TEST_COMMON_GATES_HPP

// C++ Stdlib Includes
#include <string>

// Library Includes
// (none)

// Project Includes
#include <circsim/components/Circuit.hpp>


const std::string NOR_DEF = "\
{\
    \"name\": \"NOR\",\
    \"transistors\": [\
        {\
            \"id\": 1,\
            \"name\": \"trans_a\",\
            \"type\": \"NMOS\",\
            \"gate_wire\": 1,\
            \"source_wire\": 3,\
            \"drain_wire\": 0\
        },\
        {\
            \"id\": 2,\
            \"name\": \"trans_b\",\
            \"type\": \"NMOS\",\
            \"gate_wire\": 2,\
            \"source_wire\": 3,\
            \"drain_wire\": 0\
        }\
    ],\
    \"wires\": [\
        {\
            \"id\": 0,\
            \"special\": \"GND\",\
            \"ctrl_transistors\": [ 1, 2 ],\
            \"gate_transistors\": []\
        },\
        {\
            \"id\": 1,\
            \"name\": \"IN_A\",\
            \"ctrl_transistors\": [],\
            \"gate_transistors\": [ 1 ]\
        },\
        {\
            \"id\": 2,\
            \"name\": \"IN_B\",\
            \"ctrl_transistors\": [],\
            \"gate_transistors\": [ 2 ]\
        },\
        {\
            \"id\": 3,\
            \"name\": \"OUT\",\
            \"pulled\": \"HIGH\",\
            \"ctrl_transistors\": [ 1, 2 ],\
            \"gate_transistors\": []\
        }\
    ]\
}";

circsim::components::Circuit create_nand();
circsim::components::Circuit create_nor();

#endif // #ifndef __CIRCSIM_TEST_COMMON_GATES_HPP
