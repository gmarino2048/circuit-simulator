#ifndef __CIRCSIM_COMPONENTS_TRANSISTOR_HPP
#define __CIRCSIM_COMPONENTS_TRANSISTOR_HPP

#include <string>
#include <circsim/components/Wire.hpp>

namespace circsim::components {

class Transistor {
protected:

    size_t _id;
    std::string _name;

    Wire *_source;
    Wire *drain;
    Wire *_gate;


};

}

#endif//__CIRCSIM_COMPONENTS_TRANSISTOR_HPP