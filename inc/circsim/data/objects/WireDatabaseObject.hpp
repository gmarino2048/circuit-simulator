/**
 * @file WireDatabaseObject.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief The declaration for the Wire Database Object class
 * @version 0.1
 * @date 2022-03-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_DATA_OBJECTS_WIREDATABASEOBJECT_HPP
#define __CIRCSIM_DATA_OBJECTS_WIREDATABASEOBJECT_HPP

// C++ Stdlib Includes
// (none)

// Library Includes
#include <sqlite3.h>

// Project includes
#include <circsim/components/Wire.hpp>
#include <circsim/data/DatabaseObject.hpp>

namespace circsim::data::objects
{

/**
 * @brief The wire database object used to convert individual
 *        wires to objects within an SQL database.
 * 
 */
class WireDatabaseObject final : public DatabaseObject
{
private:

    /// The internal wire object used as reference by this class
    components::Wire _wire_object;

    /// The database table name for wire objects
    static const char *_wire_table_name;

    static const char *_wire_pkey_col_name;

    /// The columns to be used in the database itself
    static const std::vector<DbColumn> _columns;


    /**
     * @brief Get the Db table columns for the Wire object
     * 
     * @return std::vector<DbColumn> The wire object columns
     */
    std::vector<DbColumn> _get_columns() const override;

    /**
     * @brief Get the primary key value of the wire
     * 
     * @return DbValue The primary key value
     */
    DbValue _get_primary_key() const override;

    /**
     * @brief Get the wire object values
     * 
     * @return std::vector<DbValue> The values of the wire object
     */
    std::vector<DbValue> _get_values() const override;

public:

    /**
     * @brief Default constructor
     * 
     */
    WireDatabaseObject();

    /**
     * @brief Constructs a new WireDatabaseObject class using
     *        an existing wire object.
     * 
     * @param wire T
     */
    WireDatabaseObject(const components::Wire &wire);


    /**
     * @brief Get the wire object
     * 
     * @return components::Wire This instance's wire object
     */
    inline components::Wire wire() const { return _wire_object; }

    /**
     * @brief Set the wire object
     * 
     * @param wire The new wire object for this instance
     */
    inline void wire(const components::Wire &wire) { _wire_object = wire; }


    /**
     * @brief Import a wire object from a single row in a statement, and
     *        assign that wire object to this database object.
     * 
     * @param statement The SQLite statement to fetch the row from.
     */
    void import(sqlite3_stmt *statement) override;

};

}

#endif  // #ifndef __CIRCSIM_DATA_OBJECTS_WIREDATABASEOBJECT_HPP
