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

    /**
     * @brief This class acts as a handle for a SQLite Statement which
     *        will automatically finalize the enclosed statement once it
     *        goes out of scope.
     * 
     */
    class SqliteStatement final
    {
    private:

        /// The Sqlite Statement object retained by this object
        sqlite3_stmt* _statement = nullptr;

        /**
         * @brief Delete the default constructor.
         * 
         */
        SqliteStatement() = delete;

        /**
         * @brief Delete the copy constructor.
         * 
         * @param other The other instance to copy from
         */
        SqliteStatement(const SqliteStatement& other) = delete;

        /**
         * @brief Delete the copy assignment operator.
         * 
         * @param other The other instance to copy from.
         * @return SqliteStatement& A reference to this instance
         */
        SqliteStatement& operator=(const SqliteStatement& other) = delete;

    public:

        /**
         * @brief Automatic conversion constructor from sqlite3_stmt.
         * 
         */
        SqliteStatement(sqlite3_stmt* statement);

        /**
         * @brief Move constructor.
         * 
         * @param other The object to move the information from
         */
        SqliteStatement(SqliteStatement&& other) noexcept;

        /**
         * @brief Call sqlite3_finalize on the object's pointer when it goes
         *        out of scope.
         * 
         */
        ~SqliteStatement();


        /**
         * @brief Move assignment operator.
         * 
         * @param other The other reference to move from
         * @return SqliteStatement& A reference to this object
         */
        SqliteStatement& operator=(SqliteStatement&& other) noexcept;

        /**
         * @brief Automatic conversion pointer to a sqlite3_stmt pointer.
         * 
         * @return sqlite3_stmt* The raw pointer to the sqlite3 statement
         */
        operator sqlite3_stmt*();
    };

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
     * @brief Create a sqlite3 statement and bind all the given
     *        values to it. Return the handle for the statement
     *        once it's ready to be used.
     * 
     * @param query The statement to be compiled
     * @param values The values to bind to the statement
     * @return SqliteStatement The handle for the compiled statement
     */
    SqliteStatement _bind_values
    (
        const std::string& query,
        const std::vector<SqlValue>& values
    ) const;

    /**
     * @brief Get the object name for working with database tables.
     * 
     * @tparam T The type of object
     * @return std::string The table name of the object
     */
    template<class T>
    static const std::string _table_name();

    /**
     * @brief Check to see if the table exists in the database, otherwise
     *        create it.
     * 
     * @tparam T The type of object to be stored in the table
     * @return true The database contains the requested table
     * @return false The database does not contain the requested table
     */
    template<class T>
    bool _table_exists();

    /**
     * @brief Creates a table to contain the specified object type
     * 
     * @tparam T The type of object to make the table for
     */
    template<class T>
    void _create_table();

    /**
     * @brief Creates all the tables of the database if they don't already
     *        exist.
     * 
     */
    void _create_tables();


    /**
     * @brief Encode the provided object as a sequence of SQL
     *        column values.
     * 
     * @tparam T The type of the object to incode
     * @param object The object instance to encode
     * @return std::vector<SqlValue> The sequence of encoded values
     */
    template<class T>
    std::vector<SqlValue> _encode(const T& object) const;

    /**
     * @brief Turn a retrieved set of values back into an object
     *        instance.
     * 
     * @tparam T The type of object to convert back to
     * @param statement The prepared SQL row to decode
     * @return T The object instance converted from the raw SQL values
     */
    template<class T>
    T _decode(SqliteStatement& statement) const;

public:

    /**
     * @brief Default constructor
     * 
     * This will automatically create the database in-memory as opposed to
     * on the disk. This method is not persistent, but is extremely useful
     * when testing different commands in automation.
     * 
     */
    ExternalStorage();

    /**
     * @brief Constructs a new database handle using the file at
     *        the provided path. If the file does not exist, it
     *        will be created.
     * 
     * This method will create the database on disk. If this behavior is
     * not desired, then you will need to use the default constructor instead.
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
    T get(const uint64_t id) const;

    /**
     * @brief Get all objects of the specified type
     * 
     * @tparam T The type of object to fetch
     * @return std::vector<T> A list of all objects stored in the database
     */
    template<class T>
    std::vector<T> get_all() const;
};

}

#endif // #ifndef __CIRCSIM_DATA_EXTERNALSTORAGE_HPP