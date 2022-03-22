/**
 * @file ValueError.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Contains the definition for the ValueError class.
 * @version 0.1
 * @date 2022-02-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_COMMON_VALUEERROR_HPP
#define __CIRCSIM_COMMON_VALUEERROR_HPP

// C++ Stdlib Includes
#include <exception>
#include <string>

// Library Includes
// (none)

// Project Includes
// (none)

namespace circsim::common
{

/**
 * @brief An exception meant to be used when the input to
 *        some method and/or process is malformed.
 * 
 */
class ValueError final : public std::exception
{
private:

    /// The message contained within this exception
    std::string _message;

public:

    /// Remove the default constructor to require a message
    ValueError() = delete;

    /// Only constructor for the exception requires message initialization
    ValueError(const std::string &message) : _message(message) { }


    /// Get a string representation of the message
    std::string message() const noexcept { return _message; }

    /// Get a c-string representation of the message
    const char *what() const noexcept { return _message.c_str(); }

};

}

#endif
