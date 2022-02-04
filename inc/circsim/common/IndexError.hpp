/**
 * @file IndexError.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Contains the declarations for IndexError.
 * @version 0.1
 * @date 2022-02-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <exception>
#include <string>

namespace circsim::common
{

/**
 * @brief This exception is thrown when there is a problem with element
 *        indexing within the database. Most commonly when there is a
 *        duplicate ID.
 * 
 */
class IndexError final : public std::exception
{
private:

    /// Internal container for the message
    std::string _message;

public:

    /// Delete default constructor
    IndexError() = delete;

    /// Instantiate this exception with a message
    IndexError(const std::string& message): _message(message) { }


    /// Message getter
    inline std::string message() const noexcept { return _message; }

    /// Override std::exception error message
    inline const char* what() const noexcept override { return _message.c_str(); }

};

}