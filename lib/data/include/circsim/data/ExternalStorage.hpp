/**
 * @file ExternalStorage.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief The external storage unit is responsible for storing processor
 *        information in a more long-term format. This file contains the
 *        object definitions for the External storage unit.
 * @version 0.1
 * @date 2022-07-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_DATA_EXTERNALSTORAGE_HPP
#define __CIRCSIM_DATA_EXTERNALSTORAGE_HPP

// C++ Stdlib Includes
#include <cstdint>
#include <filesystem>
#include <string>
#include <variant>
#include <vector>

// Library Includes
#include <sqlite3.h>

namespace circsim::data
{

/**
 * @brief This class defines the object responsible for storing circuit
 *        information in a persistent manner. Currently, this involves
 *        putting all of the components and state into a SQL database
 *        which can be accessed and updated later.
 * 
 */
class ExternalStorage final
{
public:

    /// Define the different values that can be stored in the database
    typedef std::variant
    <
        std::nullptr_t,
        int32_t,
        int64_t,
        double,
        std::string, 
        std::vector<uint8_t>
    > SqlValue;

private:

    /// The database connection handle
    sqlite3* _db_connection_obj;

    /// The path to the database file
    std::filesystem::path db_object_path;


    /**
     * @brief Convert the given value into one that can be
     *        stored in the database. This method is useful
     *        for complex types which may not have a
     *        canonical representation in the database library.
     * 
     * @tparam T The type of the value to be converted
     * @param value The value to convert
     * @return SqlValue The resulting object containing a database-friendly
     *                  representation of the provided value.
     */
    template<class T>
    static SqlValue _to_sql_type(const T value);

    /**
     * @brief Convert a database-friendly value back into the original
     *        format. Intended for retrieving objects from the database.
     * 
     * @tparam T The type of the value to convert into
     * @param value The value pulled from the database
     * @return T The converted value from the database
     */
    template<class T>
    static T _from_sql_type(const SqlValue& value);

    /**
     * @brief Creates a table to contain the specified object type
     * 
     * @tparam T The type of object to make the table for
     */
    template<class T>
    void _create_table();

public:

    /**
     * @brief Default constructor
     * 
     */
    ExternalStorage();

    /**
     * @brief Constructs a new database handle using the file at
     *        the provided path. If the file does not exist, it
     *        will be created.
     * 
     * @param db_path The path to the database file
     */
    ExternalStorage(const std::filesystem::path& db_path);

    /// Remove the copy constructor
    ExternalStorage(const ExternalStorage& copy) = delete;

    /// Remove the move constructor
    ExternalStorage(ExternalStorage&& move) noexcept = delete;


    /// Remove the copy operator
    ExternalStorage& operator=(const ExternalStorage& copy) = delete;

    /// remove the move operator
    ExternalStorage& operator=(ExternalStorage&& move) noexcept = delete;


    /**
     * @brief Destructor. Closes the database object.
     * 
     */
    ~ExternalStorage();


    /**
     * @brief Return the count of objects of the specified type.
     * 
     * @tparam T The type of objects to fetch
     * @return size_t The count of the objects
     */
    template<class T>
    size_t count() const;


    /**
     * @brief Checks to see if the database contains the
     *        object with a matching primary ID as the one
     *        provided.
     * 
     * @tparam T The type of object to search for
     * @param object The object to use as a reference
     * @return true An object with a matching ID exists in the database
     * @return false No object with a matching ID exists in the database
     */
    template<class T>
    bool contains(const T& object) const;

    /**
     * @brief Check to see if the database contains an object which
     *        exactly matches the provided object
     * 
     * @tparam T The type of object to check for
     * @param object The object to search for and compare against
     * @return true An object exactly matching this one exists in the database
     * @return false No object exactly matching this one exists in the database
     */
    template<class T>
    bool contains_current(const T& object) const;


    /**
     * @brief Adds the given value to the database only if it does not
     *        exist already. If an item with a matching ID exists in the
     *        database, this method will throw an error.
     * 
     * @tparam T The type of object to add
     * @param object The object to add to the database
     */
    template<class T>
    void add_component(const T& object);

    /**
     * @brief Adds the given value to the database. If an object with a matching
     *        ID already exists in the database, it will be updated with the
     *        provided object's value.
     * 
     * @tparam T The type of object to add to the database
     * @param object The object to be added
     */
    template<class T>
    void update_component(const T& object);

    /**
     * @brief Fetch an object from the database.
     * 
     * @tparam T The type of object to get
     * @return T The object returned from the database
     */
    template<class T>
    T get(const size_t id) const;
};

}

#endif // #ifndef __CIRCSIM_DATA_EXTERNALSTORAGE_HPP