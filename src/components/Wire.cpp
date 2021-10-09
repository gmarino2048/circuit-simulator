
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

   