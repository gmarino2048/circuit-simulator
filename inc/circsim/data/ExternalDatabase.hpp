/**
 * @file ExternalDatabase.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief The object definition for the external database object
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_DATA_EXTERNALDATABASE_HPP
#define __CIRCSIM_DATA_EXTERNALDATABASE_HPP

// C++ stdlib includes
#include <filesystem>
#include <string>
#include <vector>

// Third-party includes
#include <sqlite3.h>

// Project includes
#include <circsim/data/DatabaseObject.hpp>

namespace circsim::data
{

/**
 * @brief This class is responsible for creating and maintaining
 *        the external database used to store the state of the
 *        processor for further analysis.
 * 
 */
class ExternalDatabase final
{
private:

    /// The connection object to the sqlite database
    ::sqlite3 *_db_connection_obj;

    /// The filepath to the database file
    std::filesystem::path _db_filepath;

    /**
     * @brief Look in the database and see if the table exists
     * 
     * @param table_name The name of the table to look for
     * @return true if the table of data exists
     */
    bool _table_exists(const std::string &table_name);

public:

    /**
     * @brief Default constructor for the external database
     * 
     */
    ExternalDatabase();

    /**
     * @brief Constructor to make an open the external database
     * 
     * @param db_file The database file to use
     * @param overwrite Whether to overwrite the existing file
     */
    ExternalDatabase
    (
        const std::filesystem::path db_file,
        const bool overwrite = true
    );


    /**
     * @brief Copy constructor for the external database
     * 
     * @param other The object to copy from
     */
    ExternalDatabase(const ExternalDatabase &other);

    /**
     * @brief Move constructor for the external database
     * 
     * @param other The object reference to move from
     */
    ExternalDatabase(ExternalDatabase &&other) noexcept;


    /**
     * @brief The destructor for the external database object
     * 
     */
    ~ExternalDatabase();


    /**
     * @brief Open the database file on the disk
     * 
     * @param db_file The database file to open
     * @param overwrite Overwrite this file (otherwise data
     *                  will be appended to existing objects)
     */
    void open
    (
        const std::filesystem::path db_file,
        const bool overwrite = true
    );

    /**
     * @brief Close the database file if it's open, otherwise
     *        do nothing
     * 
     */
    void close();


    /**
     * @brief Create a table for the provided type of object
     * 
     * @param object The object to create a table for
     */
    void create_table(const DatabaseObject *object);

    /**
     * @brief Insert a single element into the table
     * 
     * @param object The object to insert to the table
     */
    void insert(const DatabaseObject *object);

    /**
     * @brief Insert all elements to the table. All of the elements
     *        in the list must be of the same object type
     * 
     * @param objects The list of objects to insert
     */
    void insert_all(const std::vector<DatabaseObject*> &objects);


    /**
     * @brief Copy operator for the external database
     * 
     * @param other The other reference to copy from
     * @return ExternalDatabase& The reference to the copy
     */
    ExternalDatabase& operator=(const ExternalDatabase &other);
    
    /**
     * @brief Move operator for the external database
     * 
     * @param other The other reference to move from
     * @return ExternalDatabase& The reference to the moved object
     */
    ExternalDatabase& operator=(ExternalDatabase &&other) noexcept;

};

}

#endif // #ifndef __CIRCSIM_DATA_EXTERNALDATABASE_HPP