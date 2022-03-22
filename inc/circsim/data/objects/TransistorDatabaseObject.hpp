/**
 * @file TransistorDatabaseObject.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief This contains the transistor database object class
 *        declaration.
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CIRCSIM_DATA_OBJECTS_TRANSISTORDATABASEOBJECT_HPP
#define __CIRCSIM_DATA_OBJECTS_TRANSISTORDATABASEOBJECT_HPP

// C++ Stdlib Includes
// (none)

// Library Includes
// (none)

// Project includes
#include <circsim/components/Transistor.hpp>
#include <circsim/data/DatabaseObject.hpp>

namespace circsim::data::objects
{

/**
 * @brief Database Object intended to convert transistor objects
 *        to SQL format.
 * 
 */
class TransistorDatabaseObject final : public DatabaseObject
{
private:

    /// The transistor object to convert
    components::Transistor _transistor_object;

    /// The table name for all transistors
    static const char *_transistor_table_name;

    /// The transistor primary key column name
    static const char *_transistor_pkey_col_name;

    /// The list of columns for this object
    static const std::vector<DbColumn> _columns;


    /**
     * @brief Get the list of columns for the Transistor
     *        object
     * 
     * @return std::vector<DbColumn> The list of columns in the database
     */
    std::vector<DbColumn> _get_columns() const override;

    /**
     * @brief Get the primary key for this object
     * 
     * @return DbValue The primary key for this object
     */
    DbValue _get_primary_key() const override;

    /**
     * @brief Get the list of values for this transistor object.
     * 
     * @return std::vector<DbValue> The list of values
     */
    std::vector<DbValue> _get_values() const override;

public:

    /**
     * @brief Default contructor
     * 
     */
    TransistorDatabaseObject();

    /**
     * @brief Construct a new object given an existing Transistor
     * 
     * @param transistor The transistor used to construct this object
     */
    TransistorDatabaseObject(const components::Transistor &transistor);


    /**
     * @brief Get this object's transistor
     * 
     * @return components::Transistor 
     */
    inline components::Transistor transistor()
    { 
        return _transistor_object;
    }

    /**
     * @brief set this object's transistor
     * 
     * @param transistor The new transistor object
     */
    inline void transistor(const components::Transistor &transistor)
    {
        _transistor_object = transistor;
    }

};

}

#endif // #ifndef __CIRCSIM_DATA_OBJECTS_TRANSISTORDATABASEOBJECT_HPP
