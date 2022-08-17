/**
 * @file JsonParser.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Declaration file for the circuit simulator JSON parser.
 * @version 0.1
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_DATA_JSONPARSER_HPP
#define __CIRCSIM_DATA_JSONPARSER_HPP

// C++ Stdlib Includes
#include <filesystem>
#include <string>

// Library Includes
#include <boost/json.hpp>

// Project Includes
#include <circsim/components/Circuit.hpp>

namespace circsim::data
{

/**
 * @brief This class is responsible for parsing the Circuit object
 *        from a given JSON specification.
 * 
 */
class JsonParser final
{
private:

    /// The parser object used to decode the raw JSON object
    boost::json::parser _parser;


    /**
     * @brief Convert a JSON value to a circuit simulator object.
     * 
     * @tparam T The type of object to convert to
     * @param value The JSON value parsed from the input
     * @return T The C++ value of the given JSON object
     */
    template<class T>
    static T convert_object(const boost::json::value& value);

public:

    /**
     * @brief Parse a circuit from a JSON string.
     * 
     * @param json_string The string to parse information from
     * @return components::Circuit The circuit object defined by the JSON string
     */
    static components::Circuit parse(const std::string& json_string);

    /**
     * @brief Parse a circuit from a JSON file.
     * 
     * @param json_filepath The filepath to the JSON object
     * @return components::Circuit The circuit object defined by the JSON file
     */
    static components::Circuit parse(const std::filesystem::path& json_filepath);

};

}

#endif // #ifndef __CIRCSIM_DATA_JSONPARSER_HPP
