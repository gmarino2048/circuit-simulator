/**
 * @file EndianOperations.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Contains operations to convert to and from network endianness
 *        for cross-system compatibility.
 * @version 0.1
 * @date 2022-07-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_COMMON_ENDIANOPERATIONS_HPP
#define __CIRCSIM_COMMON_ENDIANOPERATIONS_HPP

namespace circsim::common
{

class EndianOperations
{
private:

    enum Endianness
    {
        BIG,
        LITTLE
    };

    static Endianness _determine_endianness();

    template<class T>
    static T _byteswap(const T value);

public:

    template<class T>
    static T host_to_big_endian(const T value);

    template<class T>
    static T big_endian_to_host(const T value);

    EndianOperations() = delete;
    virtual ~EndianOperations() = 0;

};

}

#endif // #ifndef __CIRCSIM_COMMON_ENDIANOPERATIONS_HPP