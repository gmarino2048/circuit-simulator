#ifndef __CIRCSIM_COMPONENTS_TRANSISTOR_HPP
#define __CIRCSIM_COMPONENTS_TRANSISTOR_HPP

#include <string>
#include <circsim/components/Wire.hpp>

namespace circsim::components {

class Transistor {
public:

    typedef Wire::State WireState;

protected:

    ssize_t _id;
    std::string _name;

    size_t _gate_id;
    size_t _source_id;
    size_t _drain_id;

    WireState _gate_state;

    Wire *_source_wire_cached = nullptr;
    Wire *_drain_wire_cached = nullptr;

public:

    Transistor();

    Transistor(
        const size_t id,
        const size_t gate_id,
        const size_t source_id,
        const size_t drain_id
    );

    Transistor(
        const size_t id,
        const std::string &name,
        const size_t gate_id,
        const size_t source_id,
        const size_t drain_id
    );

    WireState gate_state() const;
    WireState source_state() const;
    WireState drain_state() const;

    virtual void recalc() = 0;

};

}

#endif//__CIRCSIM_COMPONENTS_TRANSISTOR_HPP