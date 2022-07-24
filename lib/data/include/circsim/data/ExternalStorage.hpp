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

class ExternalStorage final
{
public:

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

    sqlite3* _db_connection_obj;
    std::filesystem::path db_object_path;

    template<class T>
    static SqlValue _to_sql_type(const T value);

    template<class T>
    static T _from_sql_type(const SqlValue& value);

    template<class T>
    void _create_column();

    template<class T>
    void _store(const T &value);

    template<class T>
    void _store_all(const std::vector<T>& values);

    template<class T>
    void _load(const T &value);

    template<class T>
    void _load_all(const std::vector<T> &value);

public:

    ExternalStorage();

    ExternalStorage(const std::filesystem::path& db_path);

    ExternalStorage(const ExternalStorage& copy) = delete;
    ExternalStorage(ExternalStorage&& move) noexcept = delete;

    ExternalStorage& operator=(const ExternalStorage& copy) = delete;
    ExternalStorage& operator=(ExternalStorage&& move) noexcept = delete;

    ~ExternalStorage() = default;

    template<class T>
    size_t count() const;

    template<class T>
    bool contains(const T& object) const;

    template<class T>
    bool contains_current(const T& object) const;

    template<class T>
    void add_component(const T& object);

    template<class T>
    void update_component(const T& object);

    template<class T>
    T get() const;
};

}

#endif // #ifndef __CIRCSIM_DATA_EXTERNALSTORAGE_HPP