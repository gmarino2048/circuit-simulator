# Circuit Simulator

This project provides a relatively fast and flexible Circuit Simulator.
Note that this is NOT an emulator, but instead a gate-level simulation of the chip state.
Accuracy is prioritized over performance, so it should not be used in situations where emulation is desired.

## JSON Circuit Specification

The circuit can be imported into the simulator using a human-readable JSON file.
This section defines the format for all of the components within the file.

### Wire

- `"id" (required) -> num`
- `"name" (optional) -> string`
- `"alternate_names" (optional) -> [string]`
- `"pulled" (optional) -> string`, one of:
  - `"HIGH"`
  - `"LOW"`
  - `"NONE" (default)`
- `"special" (optional) -> string`, one of:
  - `"VCC"`
  - `"GND"`
  - `"NONE" (default)`
- `"ctrl_transistors" (required) -> [num]`
- `"gate_transistors" (required) -> [num]`

### Transistor

- `"id" (required) -> num`
- `"name" (optional) -> string`
- `"type" (required) -> string`, one of:
  - `"NMOS"`
  - `"PMOS"`
- `"gate_wire" (required) -> num`
- `"source_wire" (required) -> num`
- `"drain_wire" (required) -> num`

### Circuit

To decode the schema of the desired subtype, please see the references for the objects above.

- `"name" (optional) -> string`
- `"transistors" (required) -> [Transistor]`
- `"wires" (required) -> [Wire]`
