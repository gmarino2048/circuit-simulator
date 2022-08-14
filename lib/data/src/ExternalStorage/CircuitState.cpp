/**
 * @file CircuitState.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Implementation for storing the circuit state object within the external
 *        storage object.
 * @version 0.1
 * @date 2022-08-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// C++ Stdlib Includes
// (none)

// Library Includes
#include <sqlite3.h>

// Project Includes
#include <circsim/common/StateError.hpp>
#include <circsim/components/CircuitState.hpp>
#include <circsim/data/ExternalStorage.hpp>

using CircuitState = circsim::components::CircuitState;
using namespace circsim::data;


