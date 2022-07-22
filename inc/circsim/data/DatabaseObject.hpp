/**
 * @file DatabaseObject.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief The definition for the DatabaseObject interface
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_DATA_DATABASEOBJECT_HPP
#define __CIRCSIM_DATA_DATABASEOBJECT_HPP

// C++ Stdlib includes
#include <cstdint>
#include <string>
#include <vector>

// Library Includes
#include <sqlite3.h>

// Project Includes
// (none)

namespace circsim::data
{

/**
 * @brief This class provides the base class for all external database
 *        information.
 * 
 * This class is designed to provide a common interface for all
 * information written out to the internal database. It should have
 * multiple derivations, each of which handles the a distinct data type's
 * formatting within the external database.
 * 
 * NOTE: BLOB data format is not supported at this time.
 * 
 */
class DatabaseObject
{
// Public Datatypes
public:

    /**
     * @brief This enumerator holds all of the supported types for
     *        Sqlite3, so that we can ensure type consistency when
     *        constructing queries.
     * 
     */
    enum DbType
    {
        DBT_NULL,       //!< Sqlite3 NULL Data Type
        DBT_INTEGER,    //!< Sqlite3 Integer Data Type
        DBT_REAL,       //!< Sqlite3 Real Number Data Type
        DBT_TEXT,       //!< Sqlite3 Text Object Data Type
        DBT_BLOB        //!< Sqlite3 BLOB (Binary Large OBject) Type
    };

    /**
     * @brief This structure holds the information required for a single
     *        column within an SQLite table, which are the column name
     *        and type.
     * 
     */
    struct DbColumn
    {
        /// The name of the SQLite3 column
        std::string name;

        /// The Datatype that the column supports
        DbType type;
    };

    /**
     * @brief This structure holds the information required for a single
     *        entry in a database column. It contains the type for
     *        validation against the column and a string for formulating
     *        the SQL query.
     * 
     */
    struct DbValue
    {
        /// The type of the value contained in this structure
        DbType type;

        /// The string value used to construct the SQL instruction
        std::string value;
    };

// Instance Data
protected:

// Static helper methods

    /**
     * @brief Validate the input for a null value
     * 
     * @param value The value to validate
     * @return true if the value is valid for this Sqlite3 data type
     */
    static bool _validate_null(const std::string &value);

    /**
     * @brief Validate the input for a integer value
     * 
     * @param value The value to validate
     * @return true if the value is valid for this Sqlite3 data type
     */
    static bool _validate_integer(const std::string &value);

    /**
     * @brief Validate the input for a real value
     * 
     * @param value The value to validate
     * @return true if the value is valid for this Sqlite3 data type
     */
    static bool _validate_real(const std::string &value);

    /**
     * @brief Validate the input for a text value
     * 
     * @param value The value to validate
     * @return true if the value is valid for this Sqlite3 data type
     */
    static bool _validate_text(const std::string &value);

// Instance helper methods

    /// The number of columns for this object
    const size_t _num_columns;

    /// The table name used to store objects of this type
    const std::string _table_name;

    /// The name of the primary key column (must be integer)
    const std::string _pkey_col_name;

    /**
     * @brief Get the column definitions for this object
     * 
     * @return std::vector<DbColumn> A list of column definitions
     */
    virtual std::vector<DbColumn> _get_columns() const = 0;


    /**
     * @brief Get the primary key value of this object
     * 
     * @return DbValue The value of the primary key
     */
    virtual DbValue _get_primary_key() const = 0;

    /**
     * @brief Get the values for each of the columns
     * 
     * The values returned from this function must match the typing of
     * the provided column list 1:1. Otherwise, an error will be raised
     * when attempting to run this method.
     * 
     * @return std::vector<DbValue> 
     */
    virtual std::vector<DbValue> _get_values() const = 0;


    /**
     * @brief Helper method for checking database values against
     *        the database column specification
     * 
     * @param pkey_value The value of the primary key
     * @param columns The columns of the specification
     * @param values The values to check against
     */
    static void _check_values
    (
        const DbValue &pkey_value,
        const std::vector<DbColumn> &columns,
        const std::vector<DbValue> &values
    );

public:

// Static helper methods

    /**
     * @brief Validate the name of a table column
     * 
     * @param col_name The name of the column
     * @return true if the column is valid
     */
    static bool validate_column(const std::string &col_name);

    /**
     * @brief Validate an input value for the table
     * 
     * @param db_val The input value to validate
     * @return true The input value is valid
     */
    static bool validate_value(const DbValue &db_val);

    /**
     * @brief Convert a Sqlite3 database type to a string
     * 
     * @param type The type to convert
     * @return std::string The string representation for the command
     */
    static std::string type_to_string(const DbType type);

    /**
     * @brief Convert a database input value to string format
     * 
     * @tparam T The C++ type of the object to convert
     * @param object The instance of the object to convert
     * @return DbValue The value resulting from this conversion
     */
    template<class T>
    static DbValue format_value(const T &object);

    /**
     * @brief Convert the input type to the output type, with added
     *        bounds and format checking for safety.
     * 
     * @tparam T the type to decode from the 
     * @param statement The SQLite statement to read information from
     * @param col_number The column number of the SQLite Statement
     * @return T The output value determined by the type parameters
     */
    template<class T>
    static T decode_value(sqlite3_stmt *statement, const uint32_t col_number);

// Instance methods

    /**
     * @brief DatabaseObject constructor
     * 
     * @param ncols The number of columns for this object
     * @param table_name The name of the object's table
     * @param pkey_column_name The primary key column name
     */
    DatabaseObject
    (
        const size_t ncols,
        const std::string &table_name,
        const std::string &pkey_column_name
    );

    /**
     * @brief Pure virtual destructor
     * 
     * Prevents initialization of this object.
     * 
     */
    virtual ~DatabaseObject() = 0;


    /**
     * @brief Validate all the columns in this object,
     *        including the primary key.
     * 
     * If this method returns, then the values are valid.
     * If there are any invalid values, then this method will
     * throw an exception.
     * 
     */
    void check_columns() const;

    /**
     * @brief Check the values to ensure that they align with
     *        what's expected from the columns.
     * 
     * If this method returns, then the values are valid.
     * If there are any invalid values, then this method will
     * throw an exception.
     * 
     */
    void check_values() const;

    /**
     * @brief Check the values of the provided object against the
     *        columns of this object.
     * 
     * If this method returns, then the values are valid.
     * If there are any invalid values, then this method will
     * throw an exception.
     * 
     * @param other The object whose values we will check against
     *              this object's columns.
     */
    void check_values_against_current(const DatabaseObject *other) const;


    /**
     * @brief Generate the SQL command to create the table
     *        for this object
     * 
     * @return std::string The SQL command to create a table
     */
    std::string create_table() const;

    /**
     * @brief Generate the SQL command to insert an item into
     *        the table.
     * 
     * @return std::string The SQL command to insert an item
     */
    std::string insert_item() const;

    /**
     * @brief Generate the SQL command to batch-insert all of the
     *        provided values into the database.
     * 
     * This method will NOT add this object to the database. The
     * current instance is only used to verify the columns match the
     * values. If you want to add this object, you will need to
     * provide a pointer to it in the provided list.
     * 
     * @param others The objects to add to the batch insertion command.
     * @return std::string The batch insertion SQL command
     */
    std::string insert_all
    (
        const std::vector<DatabaseObject*> &others
    ) const;


    /**
     * @brief Overwrite this object using the imported values
     *        from the provided statement. Note that the statement
     *        must be prepared and at least partially executed to
     *        function properly.
     * 
     * @param statement The SQLite3 statement to import from
     */
    virtual void import(sqlite3_stmt *statement) = 0;

};

}

#endif // #ifndef __CIRCSIM_DATA_DATABASEOBJECT_HPP
