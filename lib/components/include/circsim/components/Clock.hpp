/**
 * @file Clock.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief Clock module responsible for tracking ticks and changing wire
 *        status between ticks.
 * @version 0.1
 * @date 2023-02-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __CIRCSIM_COMPONENTS_CLOCK_HPP
#define __CIRCSIM_COMPONENTS_CLOCK_HPP

// C++ Stdlib Includes
#include <cinttypes>
#include <vector>

// Library Includes
// (none)

// Project Includes
#include <circsim/components/Circuit.hpp>
#include <circsim/components/Wire.hpp>

namespace circsim::components
{

/**
 * @brief This class controls timing and simulates oscillation within
 *        the circuit module. 
 * 
 */
class Clock final
{
private:

    /// The number of half-ticks currently counted by the clock
    uint64_t _halftick_counter;

    /// Current state of the clock wires
    Wire::State _current_state;

    /// List of wires to adjust
    std::vector<uint64_t> _wires;

    /// Circuit reference for performing updates
    Circuit* _circuit;

    /**
     * @brief Update all of the wires in the clock module as
     *        long as the circuit reference is properly set
     * 
     * @param new_state The new state of the clock wires.
     * 
     * @throw IndexError if the wire IDs do not exist in the
     *        circuit reference
     */
    void _update_wires(const Wire::State new_state);

public:

    /**
     * @brief Construct a new Clock object
     * 
     * @param wires The list of wires driven by the clock
     */
    Clock(std::vector<uint64_t> wires);

    /**
     * @brief Default destructor
     * 
     */
    ~Clock() = default;


    /**
     * @brief Get the list of wires connected to this
     *        clock object.
     * 
     * @return std::vector<uint64_t> The wire list
     */
    std::vector<uint64_t> wires() const
    {
        return _wires;
    }

    /**
     * @brief Update the wire list of the clock object with
     *        new values.
     * 
     * @param new_list The new list of wires to use.
     */
    void update_wires(const std::vector<uint64_t> new_list);


    /**
     * @brief Set the wire's internal circuit reference, which
     *        will be used to update wires when the clock ticks
     * 
     * @param circuit The circuit object to use as the reference
     */
    inline void set_circuit(Circuit& circuit)
    {
        _circuit = &circuit;
    }

    /**
     * @brief Clear the circuit reference, causing this object to
     *        be unable to update wire values.
     * 
     */
    inline void clear_circuit()
    {
        _circuit = nullptr;
    }


    /**
     * @brief Return the current halftick count.
     * 
     * @return uint64_t The current halftick count.
     */
    uint64_t halftick_count() const;

    /**
     * @brief Return the current full tick count, which is the
     *        number of completed full ticks, usually half the
     *        halftick count.
     * 
     * @return uint64_t The number of full ticks
     */
    uint64_t tick_count() const;

    /**
     * @brief Reset the current halftick count, and update the
     *        clock wires to their starting values.
     * 
     */
    void reset_counter();


    /**
     * @brief Perform a half tick on all clock wires, and update
     *        their values accordingly if a valid circuit reference
     *        is provided.
     * 
     */
    void halftick();

    /**
     * @brief Perform a full tick on all clock wires, and update
     *        their values accordingly if a valid circuit reference
     *        is provided.
     * 
     */
    void tick();
};

}

#endif  // #ifndef __CIRCSIM_COMPONENTS_CLOCK_HPP