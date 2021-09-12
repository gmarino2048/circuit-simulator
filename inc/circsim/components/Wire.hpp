#ifndef __CIRCSIM_COMPONENTS_WIRE_HPP
#define __CIRCSIM_COMPONENTS_WIRE_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <functional>

namespace circsim::components {

class Wire final {
public:

    /**
     * @brief Add an enumerator for special wire types like the VCC and
     *        GND rails, 
     * 
     */
    enum SpecialWireType {
        VCC,        //!< This is the high voltage rail
        GND,        //!< This is the grounding rail
        CLK         //!< This is the clock signal
    };

    /**
     * @brief One-hot encoded list of potential wire states. The original
     *        circuit simulator library was really inefficient with their
     *        assignment of wire states, so this object allows for simple
     *        value comparison centered around `FLOATING`.
     * 
     */
    enum State : uint8_t {
        GROUNDED =          0b00000001,     //!< Immutable off state
        PULLED_LOW =        0b00000010,     //!< Definitive off state
        FLOATING_LOW =      0b00000100,     //!< Indeterminate state, currently off
        FLOATING =          0b00001000,     //!< Indeterminate state
        FLOATING_HIGH =     0b00010000,     //!< Indeterminate state, currently on
        PULLED_HIGH =       0b00100000,     //!< Definitive on state
        HIGH =              0b01000000      //!< Immutable on state
    };


private:

    /// Typedef the optional external driver function so the code is cleaner
    typedef std::optional<std::function<State(State)>> extern_func_t;


    /// The ID of the wire. This will be -1 if it has not been initialized yet
    ssize_t _id;

    /// The name of the wire. This will be empty if the wire has not been initialized
    std::string _primary_name;

    /// Sometimes wires can have alternate names, we should list them here in case
    std::vector<std::string> _other_names;


    /// The current state of the wire, given as one of the above Wire States
    State _state;


    /// Tells whether this wire is potentially externally driven
    bool _externally_driven;

    /// The external function used to drive the wire (if any)
    extern_func_t _driver_function;

    /// The set of transistors which control this wire
    std::vector<size_t> _trans_ctl_ids;

    /// The set of transistors controlled in turn by this wire
    std::vector<size_t> _trans_gate_ids;

public:

    Wire();

    Wire(
        const SpecialWireType special_type,
        const std::vector<size_t> &control_transistors,
        const std::vector<size_t> &gate_transistors
    );

    Wire(
        const std::string &name,
        const std::function<State(State)> driver_func,
        const std::vector<size_t> &control_transistors,
        const std::vector<size_t> &gate_transistors
    );

    Wire(
        const size_t id,
        const std::string &name,
        const std::vector<size_t> &control_transistors,
        const std::vector<size_t> &gate_transistors,
        const State initial_state = FLOATING
    );

    bool externally_driven() const;

    State state() const;
    void state(const State new_state);

    bool low() const;
    bool high() const;

    void set_high_low(const bool new_state);

    operator std::string();
    bool operator==(const Wire &other);

};

}

#endif//__CIRCSIM_COMPONENTS_WIRE_HPP