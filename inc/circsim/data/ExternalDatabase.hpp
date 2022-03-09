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