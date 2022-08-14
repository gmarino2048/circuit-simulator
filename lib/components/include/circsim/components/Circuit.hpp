/**
 * @file Circuit.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Definition of a complete circuit object used as part of
 *        the simulator. It will contain each of the circuit's components
 *        and allow the lists to be accessed in constant time.
 * @version 0.1
 * @date 2022-08-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_COMPONENTS_CIRCUIT_HPP
#define __CIRCSIM_COMPONENTS_CIRCUIT_HPP

// C++ Stdlib Includes
#include <map>
#include <string>
#include <vector>

// Library Includes
// (none)

// Project Includes
#include <circsim/components/Transistor.hpp>
#include <circsim/components/Wire.hpp>

namespace circsim::components
{

/**
 * @brief This class contains all of the information needed for a
 *        single simulation circuit.
 * 
 */
class Circuit final
{
private:

    /// The name of the circuit
    std::string _name;


    /// The array of transistor objects in the circuit
    std::vector<Transistor> _transistor_instances;

    /// An index which maps IDs to transistor objects for faster lookup
    std::map<uint64_t, Transistor*> _transistor_index;


    /// The array of wire objects in the circuit
    std::vector<Wire> _wire_instances;

    /// An index which maps IDs to wire objects for faster lookup
    std::map<uint64_t, Wire*> _wire_index;


    /**
     * @brief Get the instance list of the requested object type.
     * 
     * @tparam T The type of object
     * @return std::vector<T>& The list of object instances
     */
    template<class T>
    std::vector<T>& _get_instances();

    /**
     * @brief Get the index map for the requested object type.
     * 
     * @tparam T The type of object
     * @return std::map<uint64_t, T*>& The index map for the object
     */
    template<class T>
    std::map<uint64_t, T*>& _get_index();


    /**
     * @brief Index a single element within the circuit storage map.
     * 
     * @tparam T The type of object to instance
     * @param object The object to be instanced
     */
    template<class T>
    void _index_element(const T& object);

    /**
     * @brief Index all elements in the storage map for the given type.
     * 
     * @tparam T The type of element to be indexed.
     */
    template<class T>
    void _index_all();

    /**
     * @brief Clear the index map for the given object type.
     * 
     * @tparam T The type of object to clear the index list for.
     */
    template<class T>
    void _clear_index();

public:

    /**
     * @brief Default constructor.
     * 
     */
    Circuit() = default;

    /**
     * @brief Construct a new Circuit object with a name.
     * 
     * @param name The circuit name
     */
    Circuit(const std::string& name);


    /**
     * @brief Copy constructor.
     * 
     * @param other The other instance
     */
    Circuit(const Circuit& other);

    /**
     * @brief Copy assignment operator.
     * 
     * @param other The other instance
     * @return Circuit& The copied instance
     */
    Circuit& operator=(const Circuit& other);

    /**
     * @brief Move constructor.
     * 
     * @param other The other instance
     */
    Circuit(Circuit&& other) noexcept;

    /**
     * @brief Move assignment operator.
     * 
     * @param other The other instance
     * @return Circuit& The moved instance
     */
    Circuit& operator=(Circuit&& other) noexcept;


    /**
     * @brief Default destructor.
     * 
     */
    ~Circuit() = default;


    /**
     * @brief Get the circuit name.
     * 
     * @return std::string The circuit name
     */
    inline std::string name() { return _name; }

    /**
     * @brief Set the circuit name.
     * 
     * @param name The new circuit name
     */
    inline void name(const std::string& name) { _name = name; }


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
     * @brief Add the component to the circuit if it does not already exist.
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
     * @brief Add all of the components to the circuit object.
     * 
     *        This method will not delete existing circuit components.
     * 
     * @tparam T The type of objects to be inserted
     * @param object_list The list of objects to insert
     */
    template<class T>
    void add_all_components(const std::vector<T>& object_list);


    /**
     * @brief Update the internal storage object with the given object.
     * 
     *        If the object already exists in storage, then the existing one
     *        will be replaced with the provided value. Otherwise, the new
     *        component will be added to the storage object and indexed.
     * 
     * @tparam T The type of object to add to the storage instance
     * @param object The instance to be added to the storage instance
     */
    template<class T>
    void update_component(const T& object);


    /**
     * @brief Get an object from the internal storage object.
     * 
     * @tparam T The type of the object to be retrieved
     * @param id The ID of the object to fetch
     * @return T* A pointer to the instance that exists in storage
     * 
     * @throw circsim::common::IndexError when no object with the corresponding
     *        ID exists in the storage object
     */
    template<class T>
    T* get(const uint64_t id) const;


    /**
     * @brief Search the storage object for an instance with the
     *        given name.
     * 
     * @tparam T The type of the instance to search for
     * @param object_name The name of the instance
     * @return T* A handle to the instance if found
     * 
     * @throws circsim::common::IndexError if an instance with the given name
     *         could not be found
     */
    template<class T>
    T* find(const std::string& object_name) const;

};  // end class Circuit

}   // end namespace circsim::components

#endif // #ifndef __CIRCSIM_COMPONENTS_CIRCUIT_HPP
