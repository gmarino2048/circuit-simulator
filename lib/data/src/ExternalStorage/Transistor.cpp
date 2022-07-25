
#include <circsim/components/Wire.hpp>
#include <circsim/data/ExternalStorage.hpp>

const char* WIRE_TABLE_NAME = "wires";

using Wire = circsim::components::Wire;
using namespace circsim::data;

// Template Instantiation of ExternalStorage::_create_platform
template<>
void ExternalStorage::_create_table<Wire>();

// Template instantiation of ExternalStorage::_store
template<>
void ExternalStorage::_store(const Wire& value);