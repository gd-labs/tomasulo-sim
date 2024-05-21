# Tomasulo

This repository contains a simulator for the Tomasulo algorithm.

## Key Structures

Consider the three `MIPS` instructions below:

```mips
div F0, F2, F4
add F10, F0, F8
sub F12, F8, F14
```

Given that `add` is dependant on `div` to provide the final value of `F0`, there needs to be a way of buffering the `add` operation so that `sub`, which has no dependency whatsoever, can proceed its execution without waiting for the stalls caused by the previous dependency.

In Tomasulo's algorithm these buffers are called **Reservation Stations**.

### Reservation Station (RS)

Reservation Stations fetch and stores instruction operands as soon as they're available. Each reservation station corresponds to one instruction. Once all source operands are available, the instruction is sent for execution, provided a functional unit is also available. Once execution is complete, the result is buffered at the reservation station.

#### Structure

Each Reservation Station is consisted on the following fields:

- **Operation to perform:** can be either `add` or `sub` if it is an addition command, or `MUL.D` or `div` if it is and multiplier command

- **Qj:** stores the RS that will produce the first operand. If `null`, the value is already available

- **Vj:** stores the value of the first operand

- **Qk:** stores the RS that will produce the second operand. If `null`, the value is already available

- **Vk:** stores the value of the second operand

- **Imm/addr:** stores immediate or effective address

- **Busy:** indicates if the RS is occupied or free

To track wheater an operand value is available on the register or if it is currently being produced, each register also has an additional field **RS** that contains the identifier of the reservation station that will produce a result that should be stored in this register. If no currently active instruction computes the result destineded for this register, this value is `0` or `null`

## Pipeline Phases

1. **Instruction Fetch**: fetch the next instruction into queue of pending ones.

2. **Issue**: get the next instruction from head of instruction queue.

   - If the matching reservation station is free, issue instruction to it.
   - If source operands are in registers, the instruction is issued with the values
   - If source operands are being produced, it is linked with the producing reservation station

3. **Execute**: when all operands are available and the functional unit is free, execute.

   - If not, monitor reservation stations and register file for operands.
   - Many instructions may be available at the same time
        - for reservation station, the next instruction to excecuted is arbitrary, usually using FIFO
        - load/store instructions are always executed in order


## Files

- **examples**: contains the test cases for use in the simulator.

- **cli**: is the command-line interface, is used to handle inputs.

- **intruction**: define the struvt of the instructions read and parser and treat it.

- **main**: is the main os the simulator, initializes the simulation, load the instruction and components.

- **simulator**: contain the  core of the simulator. This manage to fetch intructions, get them to the reservation station, execute and give back the results.

- **station**: this is the reservation station by it self, this include the logic for tracking the state, manage instructions and handle the dependencies.