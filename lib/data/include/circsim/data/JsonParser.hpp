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

class JsonParser final
{
private:

    boost::json::parser _parser;

    template<class T>
    static T convert_object(const boost::json::value& value);

public:

    static components::Circuit parse(const std::string& json_string);

    static components::Circuit parse(const std::filesystem::path& json_filepath);

};

}

#endif // #ifndef __CIRCSIM_DATA_JSONPARSER_HPP
