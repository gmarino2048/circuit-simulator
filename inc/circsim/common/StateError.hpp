/**
 * @file StateError.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Declaration file for the StateError object
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <exception>
#include <string>

#ifndef __CIRCSIM_COMMON_STATEERROR_HPP
#define __CIRCSIM_COMMON_STATEERROR_HPP

namespace circsim::common
{

/**
 * @brief An error occurring when the simulator finds itself
 *        in some invalid state.
 * 
 */
class StateError final : public std::exception
{
private:

    /// The message contained within this exception
    std::string _message;

public:

    /// Remove the default constructor to require a message
    StateError() = delete;

    /// Only constructor for the exception requires message initialization
    StateError(const std::string &message) : _message(message) { }


    /// Get a string representation of the message
    std::string message() const noexcept { return _message; }

    /// Get a c-string representation of the message
    const char *what() const noexcept { return _message.c_str(); }
};

}

#endif  // #ifndef __CIRCSIM_COMMON_STATEERROR_HPP
