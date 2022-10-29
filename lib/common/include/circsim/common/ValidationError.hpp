/**
 * @file ValidationError.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief This error is used for failed circuit validation.
 * @version 0.1
 * @date 2022-10-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_COMMON_VALIDATIONERROR_HPP
#define __CIRCSIM_COMMON_VALIDATIONERROR_HPP

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
 * @brief An error occurring when the simulator finds itself
 *        in some invalid state.
 * 
 */
class ValidationError final : public std::exception
{
private:

    /// The message contained within this exception
    std::string _message;

public:

    /// Remove the default constructor to require a message
    ValidationError() = delete;

    /// Only constructor for the exception requires message initialization
    ValidationError(const std::string &message) : _message(message) { }


    /// Get a string representation of the message
    std::string message() const noexcept { return _message; }

    /// Get a c-string representation of the message
    const char *what() const noexcept { return _message.c_str(); }
};

}

#endif  // #ifndef __CIRCSIM_COMMON_VALIDATIONERROR_HPP
