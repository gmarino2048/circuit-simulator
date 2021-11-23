
#include <cstdint>
#include <optional>
#include <iomanip>
#include <sstream>
#include <circsim/components/Wire.hpp>

using namespace circsim::components;

// Local constants
const char *ST_UNK = "UNKNOWN";
const char *ST_GND = "GROUNDED";
const char *ST_PLL = "PULLED LOW";
const char *ST_FLL = "FLOATING LOW";
const char *ST_FLT = "FLOATING";
const char *ST_FLH = "FLOATING HIGH";
const char *ST_PLH = "PULLED HIGH";
const char *ST_HGH = "HIGH";

const char *WIRE_VCC = "VCC";
const char *WIRE_GND = "GND";


/// Initialize to -1 until it is set
ssize_t Wire::_VCC_ID = -1;

/// Initialize to -1 until it is set
ssize_t Wire::_GND_ID = -1;


// Initialize special wire functions
const std::function<Wire::State(Wire::State)> Wire::_VCC_FUNC = [](State)
{
    return HIGH;
};

const std::function<Wire::State(Wire::State)> Wire::_GND_FUNC = [](State)
{
    return GROUNDED;
};


/**
 * Default constructor for the wire object. Make sure that the
 * ID of the wire is -1 so the simulator can quickly detect an error
 */
Wire::Wire(): _id(-1),
              _state(UNKNOWN),
              _pulled(false),
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
    _pulled(false),
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
    _pulled(false),
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
    const bool pulled,
    const std::vector<size_t> &control_transistors,
    const std::vector<size_t> &gate_transistors,
    const State initial_state
):  _id(id),
    _primary_name(name),
    _pulled(pulled),
    _state(initial_state),
    _externally_driven(false),
    _driver_function(std::nullopt),
    _trans_ctl_ids(control_transistors),
    _trans_gate_ids(gate_transistors)
{
    // Use default for all other members
}


void Wire::add_name(const std::string &new_name)
{
    this->_other_names.push_back(new_name);
}

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


void Wire::set_high_low(const bool new_state) noexcept
{
    this->_state = new_state ? PULLED_HIGH : PULLED_LOW;
}


// Wire operators
Wire::operator std::string() const
{
    std::stringstream ss;

    ss << "Wire \"" << this->_primary_name << "\":\n";
    ss << "\tId:\t\t0x" << std::uppercase << std::setfill('0') << std::setw(16)
        << this->_id << "\n";
    ss << "\tState:\t\t" << state_to_string(this->_state) << "\n";

    return ss.str();
}


bool Wire::operator==(const Wire &rhs) const
{
    bool equivalent = _id == rhs._id;

    equivalent &= _primary_name == rhs._primary_name;
    equivalent &= _other_names == rhs._other_names;

    equivalent &= _pulled == rhs._pulled;
    equivalent &= _state == rhs._state;
    equivalent &= _externally_driven == rhs._externally_driven;

    // Don't check function equality, it's not worth it

    equivalent &= _trans_gate_ids == rhs._trans_gate_ids;
    equivalent &= _trans_ctl_ids == rhs._trans_ctl_ids;

    return equivalent;
}


// Helper functions
const char *ERR_UNK_SPECIAL_WIRE = "Unknown special wire type specified";

void Wire::set_special_wire_id(const SpecialWireType type)
{
    ssize_t *ID = nullptr;

    switch( type )
    {
        case VCC:
            ID = &this->_VCC_ID;
            break;

        case GND:
            ID = &this->_GND_ID;
            break;

        default:
            throw std::runtime_error(ERR_UNK_SPECIAL_WIRE);
    }

    if( *ID == -1 )
    {
        *ID = this->_id;
    }
    else
    {
        throw std::runtime_error
        (
            "ID of " + this->_primary_name + "already specified"
        );
    }
}


std::string Wire::special_wire_name(const SpecialWireType type)
{
    switch( type )
    {
        case VCC:       return WIRE_VCC;
        case GND:       return WIRE_GND;
        default:
            throw std::runtime_error(ERR_UNK_SPECIAL_WIRE);
    }
}


Wire::extern_func_t Wire::special_wire_func(const SpecialWireType type)
{
    switch( type )
    {
        case VCC:       return _VCC_FUNC;
        case GND:       return _GND_FUNC;
        default:
            throw std::runtime_error(ERR_UNK_SPECIAL_WIRE);
    }
}


std::string Wire::state_to_string(const State state)
{
    switch( state )
    {
        case GROUNDED:          return ST_GND;
        case PULLED_LOW:        return ST_PLL;
        case FLOATING_LOW:      return ST_FLL;
        case FLOATING:          return ST_FLT;
        case FLOATING_HIGH:     return ST_FLH;
        case PULLED_HIGH:       return ST_PLH;
        case HIGH:              return ST_HGH;
        default:                return ST_UNK;
    }
}