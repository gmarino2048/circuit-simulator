/**
 * @file InternalDatabase.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief This file contains the template declaration for
 *        an internal database of components. Within the
 *        circuit simulation, there will be two separate
 *        databases for the transistor and wire elements.
 * @version 0.1
 * @date 2021-10-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CIRCSIM_DATA_INTERNALDATABASE_HPP
#define __CIRCSIM_DATA_INTERNALDATABASE_HPP

#include <map>
#include <vector>

#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>

namespace circsim::data
{

class InternalDatabase final
{
private:

    //
    // The physical component instances
    //

    /// Heap array to store physical instances of the transistors
    std::vector<Transistor> _transistor_instances;

    /// Heap array to store physical instances of the wires
    std::vector<Wire> _wire_instances;

    //
    // The component indices to make searching easier
    //

    /// The index of transistors with their IDs
    std::map<size_t, Transistor*> _transistor_index;

    /// The index of wires with their IDs
    std::map<size_t, Wire*> _wire_index;

    //
    // Support functions
    //

    /**
     * @brief Index the element within the internal database. The
     *        indexing process stores a pointer to the object and stores
     *        it based on the component ID.
     * 
     * @param wire The wire to index
     * @throw std::runtime_error if the ID of the object is negative
     */
    void _index_element(const Wire &wire);

    /**
     * @brief Index the element within the internal database. This stores
     *        a pointer to the element with the ID of the object.
     * 
     * @param transistor The transistor to index
     * @throw std::runtime_error if the ID of the object is negative
     */
    void _index_element(const Transistor &transistor);


    /**
     * @brief Re-index all the components in the physical arrays. This is
     *        primarily used when the database is copied to re-index the new
     *        physical addresses.
     * 
     */
    void _index_all();

    /**
     * @brief This function clears the index of component pointers.
     * 
     */
    void _clear_index();

public:

    InternalDatabase();
    InternalDatabase(const InternalDatabase &other);
    InternalDatabase(InternalDatabase &&other) noexcept;

    InternalDatabase& operator=(const InternalDatabase &rhs);
    InternalDatabase& operator=(InternalDatabase &&rhs) noexcept;

    void set_wire_count(const size_t count);
    void set_transistor_count(const size_t count);

    void add_component(const Wire &wire);
    void add_component(const Transistor &transistor);

    Wire* get_wire() const;
    Transistor* get_transistor() const;

};

}

#endif //__CIRCSIM_DATA_INTERNALDATABASE_HPP
