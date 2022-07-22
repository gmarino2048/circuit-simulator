/**
 * @file LimitError.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Contains the declaration and definition of the
 *        LimitError exception class.
 * @version 0.1
 * @date 2022-02-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_COMMON_LIMITERROR_HPP
#define __CIRCSIM_COMMON_LIMITERROR_HPP

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
 * @brief Error thrown when an internal limit is exceeded.
 * 
 */
class LimitError final : public std::exception
{

private:

    /// The error message
    std::string _message;

public:

    /// Remove the default constructor for the error
    LimitError() = delete;

    /// Instantiate this exception with a message
    LimitError(const std::string &message) : _message(message) { }

    /// Get the std::string version of the message
    inline std::string message() const noexcept { return _message; }

    /// Get a C-string representation of this error's message
    inline const char *what() const noexcept { return _message.c_str(); }

};

}

#endif