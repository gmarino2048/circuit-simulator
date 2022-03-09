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

#include <sqlite3.h>
#include <filesystem>

namespace circsim::data
{

class ExternalDatabase final
{
private:

    ::sqlite3 *_db_connection_obj;
    std::filesystem::path _db_filepath;

public:

    ExternalDatabase();

    ExternalDatabase(const ExternalDatabase &other);
    ExternalDatabase(ExternalDatabase &&other) noexcept;

    ~ExternalDatabase();

    ExternalDatabase& operator=(const ExternalDatabase &other);
    ExternalDatabase& operator=(ExternalDatabase &&other) noexcept;

};

}

#endif // #ifndef __CIRCSIM_DATA_EXTERNALDATABASE_HPP