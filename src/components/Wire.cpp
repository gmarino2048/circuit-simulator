
#include <cstdint>
#include <optional>
#include <circsim/components/Wire.hpp>

using namespace circsim::components;

/// Initialize to -1 until it is set
ssize_t Wire::_VCC_ID = -1;

/// Initialize to -1 until it is set
ssize_t Wire::_GND_ID = -1;


/**
 * Default constructor for the wire object. Make sure that the
 * ID of the wire is -1 so the simulator can quickly detect an error
 */
Wire::Wire(): _id(-1),
              _state(UNKNOWN),
              _externally_driven(false),
              _driver_function(std::nullopt)
{
    // All other fields get default constructor calls
}


/**
 * Construct a special wire object given an ID and a specific type
 */
Wire::Wire
(
    const size_t id,
    const SpecialWireType special_type,
    const std::vector<size_t> &control_transistors,
    const std::vector<size_t> &gate_transistors
):  _id(id),
    _state(UNKNOWN),
    _externally_driven(true),
    _trans_ctl_ids(control_transistors),
    _trans_gate_ids(gate_transistors)
{
    _primary_name = special_wire_name(special_type);
    _driver_function = special_wire_func(special_type);
    
    set_special_wire_id(special_type);

    _state = _driver_function.value()(_state);
}


/**
 * Create an externally-driven transistor with a custom driver function.
 */
Wire::Wire
(
    const size_t id,
    const std::string &name,
    const std::function<State(State)> driver_func,
    const std::vector<size_t> &control_transistors,
    const std::vector<size_t> &gate_transistors
):  _id(id),
    _primary_name(name),
    _state(UNKNOWN),
    _externally_driven(true),
    _driver_function(driver_func),
    _trans_ctl_ids(control_transistors),
    _trans_gate_ids(gate_transistors)
{
    // Use default constructors for other members
    // Get the first state value
    _state = _driver_function.value()(_state);
}


/**
 * Constructs a new standard wire which is internally driven
 */
Wire::Wire
(
    const size_t id,
    const std::string &name,
    const std::vector<size_t> &control_transistors,
    const std::vector<size_t> &gate_transistors,
    const State initial_state
):  _id(id),
    _primary_name(name),
    _state(initial_state),
    _externally_driven(false),
    _driver_function(std::nullopt),
    _trans_ctl_ids(control_transistors),
    _trans_gate_ids(gate_transistors)
{
    // Use default for all other members
}


// Static special wire getters
size_t Wire::VCC_ID() { return Wire::_VCC_ID; }

size_t Wire::GND_ID() { return Wire::_GND_ID; }


// Wire metadata getters/setters
ssize_t Wire::id() const noexcept { return this->_id; }

std::string Wire::primary_name() const noexcept { return this->_primary_name; }

bool Wire::externally_driven() const noexcept { return this->_externally_driven; }


void Wire::add_name(const std::string &new_name)
{
    this->_other_names.push_back(new_name);
}

std::vector<std::string> Wire::other_names() const noexcept { return this->_other_names; }


// Wire state getters/setters
Wire::State Wire::state() const noexcept { return this->_state; }

void Wire::state(const State new_state) noexcept { this->_state = new_state; } 


bool Wire::low() const noexcept
{
    static const uint8_t LOW_STATES = GROUNDED | PULLED_LOW | FLOATING_LOW;
    return (bool)(this->_state & LOW_STATES);
}

bool Wire::high() const noexcept
{
    static const uint8_t HIGH_STATES = HIGH | PULLED_HIGH | FLOATING_HIGH;
    return (bool)(this->_state & HIGH_STATES);
}


void Wire::set_high_low(const bool new_state)
{
    this->_state = new_state ? PULLED_HIGH : PULLED_LOW;
}
