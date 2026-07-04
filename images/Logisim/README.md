# Logisim Simulation Screenshots

This directory contains screenshots of the Logisim simulation for the 4-bit signed ALU project.

The screenshots show both the full ALU and the individual subcircuits used to build and verify it.

## Files

### `ALU_Full.png`

Full Logisim simulation of the complete 4-bit signed ALU.

This screenshot shows the integrated design after combining the main modules:

- Add/Subtract unit
- Absolute value / two's complement handling
- Comparator
- Multiplier
- Control and output routing

It represents the overall system-level view of the ALU.

---

### `ALU_blocks.png`

High-level block diagram of the ALU.

This screenshot is meant to make the architecture easier to understand without focusing on every gate-level detail.

It shows how the major ALU modules connect together and how the input/control signals are routed.

---

### `Logisim_ADD_SUB.png`

Logisim simulation of the addition/subtraction unit.

This module performs:

A + B when SUB = 0

A - B when SUB = 1

The subtraction operation is implemented using two's complement logic:

A - B = A + (~B) + 1

The circuit also includes overflow logic for signed two's complement arithmetic.

---

### `Logisim_AbsoulteValueGetter_2sComplementer.png`

Logisim simulation of the absolute value / two's complementer circuit.

This block is used to convert a signed 4-bit two's complement number into its magnitude when needed.

The general idea is:

If sign bit = 0: output stays the same

If sign bit = 1: invert bits and add 1

This is useful before unsigned magnitude multiplication.

---

### `Logisim_Comparator.png`

Logisim simulation of the signed comparator.

The comparator first compares the two 4-bit inputs as unsigned values, then corrects the result using the sign bits.

The signed correction logic is based on:

sameSign = NOT(A3 XOR B3)

GT = (NOT A3 AND B3) OR (sameSign AND U_GT)

EQ = U_EQ

LT = (A3 AND NOT B3) OR (sameSign AND U_LT)

Where:

- U_GT, U_EQ, and U_LT are the unsigned comparator outputs.
- A3 and B3 are the sign bits.

---

### `Logisim_Multiplier.png`

Logisim simulation of the multiplier section.

The multiplier works by first treating the inputs as magnitudes and performing unsigned multiplication.

For signed multiplication, the magnitude product is corrected using the final product sign:

ProductSign = A3 XOR B3

FinalProduct = (Pmag XOR ProductSign) + ProductSign

Here, `+ ProductSign` means binary addition, not OR.

If ProductSign = 0, the product remains unchanged.

If ProductSign = 1, the product is inverted and 1 is added, producing the 8-bit two's complement signed result.

---

## Purpose of These Screenshots

These images document the simulation stage of the project before or alongside the physical breadboard implementation.

They help show that each module was designed and tested separately before being integrated into the full ALU.

## Notes

- The project is based on 4-bit two's complement signed arithmetic.
- The signed input range is from `1000` to `0111`, which represents `-8` to `+7`.
- Some modules internally use unsigned logic first, then apply signed correction where needed.
