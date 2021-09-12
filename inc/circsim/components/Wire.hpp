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
        VCC,
        GND
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

    typedef std::optional<std::function<State(State)>> extern_func_t;

    ssize_t _id;
    std::string _name;

    State _state;

    bool _externally_driven;
    extern_func_t _driver_function;

    std::vector<size_t> _trans_ctl_ids;
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