
#include <circsim/data/ExternalStorage.hpp>

using namespace circsim::data;

// Template Initialization of ExternalStorage::_to_sql_type
template<>
SqlType ExternalStorage::_to_sql_type(const uint8_t object);

template<>
SqlType ExternalStorage::_to_sql_type(const size_t object);