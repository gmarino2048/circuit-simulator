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

#include <circsim/data/InternalStorage.hpp>

circsim::data::InternalStorage create_nand();
circsim::data::InternalStorage create_nor();

#endif // #ifndef __CIRCSIM_TEST_COMMON_GATES_HPP
