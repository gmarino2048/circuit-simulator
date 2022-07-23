/**
 * @file InternalStorage.hpp
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

#ifndef __CIRCSIM_DATA_INTERNALSTORAGE_HPP
#define __CIRCSIM_DATA_INTERNALSTORAGE_HPP

// C++ Stdlib Includes
#include <map>
#include <stdexcept>
#include <vector>

// Library Includes
// (none)

// Project Includes
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>

namespace circsim::data
{

/**
 * @brief This internal database contains the components used by the simulator. It
 *        holds the instances of the transistors and wires, as well as the indexed
 *        maps of these components relative to their IDs.
 * 
 */
class InternalStorage final
{

private:

    //
    // Convenience type definitions
    //

    /// Convenience declaration of Wire class
    using Wire = components::Wire;

    /// Convenience declaration of Transistor class
    using Transistor = components::Transistor;


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

    template<class T>
    std::vector<T>& _get_storage();

    template<class T>
    std::map<size_t, T*>& _get_index();

    template<class T>
    const std::string _get_typename() const;

    /**
     * @brief Index the desired element within the internal database.
     * 
     *        Indexing means taking a pointer to the object and storing that in a
     *        hash map with the object's ID, meaning that we have the best of both
     *        worlds in terms of mimum-size and maximum-speed object storage.
     * 
     * @tparam T The type of the object which needs to be indexed
     * @param object The object to store and add to the index maps
     */
    template<class T>
    void _index_element(const T& object);

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

    /**
     * @brief This is the default constructor of the internal database object, with
     *        optional size arguments for the transistor and wire counts for added
     *        efficiency.
     * 
     * @param wire_count The number of wires to initialize the database with
     * @param transistor_count The number of transistors to initialize the database with
     */
    InternalStorage();

    /**
     * @brief Construct a new Internal Database object with a predefined wire and
     *        transistor list.
     * 
     * @param wires The list of wires to insert
     * @param transistors The list of transistors to insert
     */
    InternalStorage
    (
        const std::vector<Wire>& wires,
        const std::vector<Transistor>& transistors
    );


    /**
     * @brief Copy constructor for the internal database object
     * 
     * @param other The other object to copy from
     */
    InternalStorage(const InternalStorage &other);

    /**
     * @brief Move constructor for the internal database object
     * 
     * @param other The other object to move from
     */
    InternalStorage(InternalStorage &&other) noexcept;


    /**
     * @brief Copy construction assignment operator
     * 
     * @param rhs The refrerence object to copy from
     * @return InternalStorage& The new copied instance
     */
    InternalStorage& operator=(const InternalStorage &rhs);

    /**
     * @brief Move construction assignment operator
     * 
     * @param rhs The reference object to move from
     * @return InternalStorage& The new moved object
     */
    InternalStorage& operator=(InternalStorage &&rhs) noexcept;


    /// Default destructor
    ~InternalStorage() = default;


    /**
     * @brief Counts the number of objects stored in this instance.
     *        Only the count of the type specified by the template parameter
     *        is returned.
     * 
     * @tparam T Find the internal object count of this type
     * @return size_t The number of T objects contained in this instance
     */
    template<class T>
    size_t count() const;


    /**
     * @brief Checks to see if the internal storage object contains the given
     *        instance (or a matching instance).
     * 
     *        Note that this method only checks the storage ID of the object,
     *        and not the contents.
     * 
     * @tparam T The type of object to search for
     * @param object The instance of the object used to find a match
     * @return true if a matching object was found
     * @return false if a matching object was not found
     */
    template<class T>
    bool contains(const T& object) const;

    /**
     * @brief Check to see if the object contained in this storage instance is
     *        up to date.
     * 
     *        This method finds the object using its unique ID, and if found compares
     *        the provided instance with the one that was found. This method will
     *        only return true if the two objects completely match each other.
     * 
     * @tparam T The type of object to search for
     * @param object The instance of the object used to find a match
     * @return true if a matching object was found
     * @return false if a matching object was not found
     */
    template<class T>
    bool contains_current(const T& object) const;


    /**
     * @brief Add the component to the database if it does not already exist.
     * 
     *        This method will automatically update the index maps with the
     *        component's ID. If the ID already exists, this method will exit
     *        with an error.
     * 
     * @tparam T The type of element to add to the storage object
     * @param object The element to add to the storage object
     * @throw IndexError if an object with that ID already exists in the storage
     *                   object
     */
    template<class T>
    void add_component(const T& object);


    /**
     * @brief Update a component in the database with the new object. If the
     *        ID does not exist, it will be added to the database.
     * 
     * @param wire The new wire instance
     */
    void update_component(const Wire &wire);

    /**
     * @brief Update a component in the database with the new object. If the
     *        ID does not exist, it will be added to the database.
     * 
     * @param transistor The new Transistor instance
     */
    void update_component(const Transistor &transistor);


    /**
     * @brief Get the wire object pointer associated with a given ID
     * 
     * @param id The ID number of the wire
     * @return Wire* The pointer to the object associated with this ID
     * @throw IndexError if the ID is not in the indexed database
     */
    Wire* get_wire(const size_t id) const;

    /**
     * @brief Get the transistor object pointer associated with a given ID
     * 
     * @param id The ID number of the transistor
     * @return Transistor* The pointer to the object associated with this ID
     * @throw IndexError if the ID is not in the indexed database
     */
    Transistor* get_transistor(const size_t id) const;

    /**
     * @brief Find the first wire associated with the given name. Note that
     *        the name must exactly match the name of the wire or its aliases.
     * 
     * @param wire_name The name of the wire
     * @return Wire* The first wire with this name
     * 
     * @throw circsim::common::IndexError if the wire with the specified name
     *        cannot be found
     */
    Wire* find_wire(const std::string &wire_name) const;

};

}

#endif //__CIRCSIM_DATA_INTERNALSTORAGE_HPP
