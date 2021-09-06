#ifndef __CIRCSIM_COMPONENTS_WIRE_HPP
#define __CIRCSIM_COMPONENTS_WIRE_HPP

#include <string>

namespace circsim::components {

class Wire {
private:

    size_t _id;
    std::string _name;

    bool _status;

};

}

#endif//__CIRCSIM_COMPONENTS_WIRE_HPP