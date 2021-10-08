
#include <circsim/components/Wire.hpp>

using namespace circsim::components;

Wire::Wire(): _id(-1),
              _state(UNKNOWN),
              _externally_driven(false),
              _driver_function(std::nullopt)
{
    // All other fields get default constructor calls
}