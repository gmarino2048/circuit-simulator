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
#include <stdexcept>
#include <vector>

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
class InternalDatabase final
{
public:

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
        inline const std::string message() const noexcept { return _message; }

        /// Override std::exception error message
        inline const char* what() const noexcept override { return _message.c_str(); }
    };

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

    /**
     * @brief This is the default constructor of the internal database object, with
     *        optional size arguments for the transistor and wire counts for added
     *        efficiency.
     * 
     * @param wire_count The number of wires to initialize the database with
     * @param transistor_count The number of transistors to initialize the database with
     */
    InternalDatabase();

    /**
     * @brief Construct a new Internal Database object with a predefined wire and
     *        transistor list.
     * 
     * @param wires The list of wires to insert
     * @param transistors The list of transistors to insert
     */
    InternalDatabase
    (
        const std::vector<Wire>& wires,
        const std::vector<Transistor>& transistors
    );


    /**
     * @brief Copy constructor for the internal database object
     * 
     * @param other The other object to copy from
     */
    InternalDatabase(const InternalDatabase &other);

    /**
     * @brief Move constructor for the internal database object
     * 
     * @param other The other object to move from
     */
    InternalDatabase(InternalDatabase &&other) noexcept;


    /**
     * @brief Copy construction assignment operator
     * 
     * @param rhs The refrerence object to copy from
     * @return InternalDatabase& The new copied instance
     */
    InternalDatabase& operator=(const InternalDatabase &rhs);

    /**
     * @brief Move construction assignment operator
     * 
     * @param rhs The reference object to move from
     * @return InternalDatabase& The new moved object
     */
    InternalDatabase& operator=(InternalDatabase &&rhs) noexcept;


    /// Default destructor
    ~InternalDatabase() = default;


    /**
     * @brief Gets the number of wires in the database.
     * 
     * This count does not exclude the uninitialized wires, and so the count
     * may overestimate the number of valid wires in the database.
     * 
     * @return size_t The wire count.
     */
    inline size_t wire_count() { return _wire_instances.size(); }

    /**
     * @brief Gets the number of transistors in the database.
     * 
     * This count does not exclude uninitialized transistors, and so the number
     * of usable transistors will always be less than or equal to this number.
     * 
     * @return size_t The transistor count
     */
    inline size_t transistor_count() { return _transistor_instances.size(); }

    /**
     * @brief Checks to see whether a wire with a matching ID exists
     *        in the database.
     * 
     * @param wire The wire to check for.
     * @return bool Whether a wire with this ID exists in the database.
     */
    bool contains(const Wire& wire) const;

    /**
     * @brief Checks to see whether a transistor with a matching ID
     *        exists in the database.
     * 
     * @param transistor The transistor to search for.
     * @return bool Whether a transistor with this ID exists in the database.
     */
    bool contains(const Transistor& transistor) const;

    bool contains_current(const Wire& wire) const;

    bool contains_current(const Transistor& transistor) const;


    /**
     * @brief Add a new wire to the database and index the element
     * 
     * @param wire The wire to add to the database
     * @throw std::runtime_error if the component with this ID already exists
     */
    void add_component(const Wire &wire);

    /**
     * @brief Add a new transistor to the database and index the element
     * 
     * @param transistor The transistor to add to the database
     * @throw std::runtime_error if the component with the ID already exists
     */
    void add_component(const Transistor &transistor);


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
     * @throw std::out_of_range if the ID is not in the indexed database
     */
    Wire* get_wire(const size_t id) const;

    /**
     * @brief Get the transistor object pointer associated with a given ID
     * 
     * @param id The ID number of the transistor
     * @return Transistor* The pointer to the object associated with this ID
     * @throw std::out_of_range if the ID is not in the indexed database
     */
    Transistor* get_transistor(const size_t id) const;

};

}

#endif //__CIRCSIM_DATA_INTERNALDATABASE_HPP
