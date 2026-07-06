# 4-Bit Signed ALU on Breadboards

A fully functional 4-bit signed ALU built using real logic ICs on breadboards.

The ALU uses 4-bit two's complement numbers and supports:

- Addition
- Subtraction
- Signed comparison
- Signed multiplication
- Overflow detection for ADD/SUB
- Arduino Nano + LCD display output

The arithmetic itself is done by hardware logic ICs. The Arduino is only used as a display interface. It reads the ALU outputs through shift registers and shows the selected result on an I2C LCD.

---

## Project Overview

This project was built in stages:

1. Logisim simulation
2. TinkerCAD breadboard-style simulation
3. Physical breadboard implementation using 74xx logic ICs
4. Arduino Nano LCD display integration

The final result is a 4-bit signed ALU that takes two 4-bit inputs, processes them using real hardware logic, and displays the selected output through LEDs and an LCD.

---

## Number Format

The ALU uses 4-bit two's complement signed numbers.

| Binary | Value |
|---|---:|
| `0000` | 0 |
| `0001` | 1 |
| `0010` | 2 |
| `0011` | 3 |
| `0100` | 4 |
| `0101` | 5 |
| `0110` | 6 |
| `0111` | 7 |
| `1000` | -8 |
| `1001` | -7 |
| `1010` | -6 |
| `1011` | -5 |
| `1100` | -4 |
| `1101` | -3 |
| `1110` | -2 |
| `1111` | -1 |

---

## Main Features

### ADD/SUB Unit

The ADD/SUB circuit is based on:

```text
A + (B XOR SUB) + SUB
```

So:

```text
SUB = 0 -> A + B
SUB = 1 -> A - B
```

The circuit also includes a signed overflow flag.

Overflow is not the same as carry out. For signed two's complement arithmetic, overflow means the result is outside the valid 4-bit signed range:

```text
-8 to +7
```

The universal ADD/SUB overflow rule used is:

```text
B_eff3 = B3 XOR SUB
Overflow = NOT(A3 XOR B_eff3) AND (A3 XOR S3)
```

---

### Absolute Value / Two's Complementer

The multiplier works on magnitudes first, so signed inputs are converted into absolute values.

The magnitude circuit follows:

```text
Magnitude = (Input XOR SignBit) + SignBit
```

If the sign bit is `0`, the number stays the same. If the sign bit is `1`, the bits are inverted and `1` is added.

---

### Signed Comparator

The comparator first uses a `74HC85` to compare the two inputs as unsigned numbers.

Then sign correction logic is applied:

```text
sameSign = NOT(A3 XOR B3)

GT = (NOT A3 AND B3) OR (sameSign AND U_GT)

EQ = U_EQ

LT = (A3 AND NOT B3) OR (sameSign AND U_LT)
```

Where `U_GT`, `U_EQ`, and `U_LT` are the unsigned comparator outputs.

---

### Signed Multiplier

The signed multiplier is built in stages:

1. Convert both signed inputs to magnitudes.
2. Multiply the magnitudes using a 4x4 unsigned multiplier.
3. Correct the final 8-bit product sign.

The unsigned magnitude multiplier uses:

```text
16 AND gates
3 partial addition stages
```

The final sign correction is:

```text
ProductSign = A3 XOR B3

FinalProduct = (Pmag XOR ProductSign) + ProductSign
```

Here, `+ ProductSign` means binary addition, not OR.

If `ProductSign = 0`, the magnitude product stays unchanged. If `ProductSign = 1`, the product is converted into its 8-bit two's complement form.

---

## Arduino Display System

The Arduino Nano does not calculate the ALU result.

It only reads hardware outputs and displays them.

The ALU outputs are connected to `74HC165` parallel-in serial-out shift registers. The Arduino reads the shift registers and displays the selected output on an I2C LCD.

The display system can show outputs such as:

- ADD/SUB result
- Comparator result
- Multiplier result
- Overflow flag
- Selected operation

This keeps the project hardware-based while still making the output easier to read.

---

## Hardware IC Count

Approximate IC count for the full signed ALU:

| IC | Quantity | Purpose |
|---|---:|---|
| `74HC283` | 8 | Adders for ADD/SUB, absolute value, multiplier stages, and signed product correction |
| `74HC86` | 7 | XOR gates for SUB logic, sign handling, and product correction |
| `74HC08` | 5 | AND gates for multiplier partial products and comparator correction |
| `74HC04` | 1 | NOT gates |
| `74HC32` | 1 | OR gates |
| `74HC85` | 1 | 4-bit comparator |

Total:

```text
23 logic ICs
```

This does not include the Arduino Nano, LCD module, shift registers, switches, LEDs, resistors, or capacitors.

---

## Repository Structure

```text
4Bit_Signed_ALU/
|
|-- Arduino/
|   |-- Arduino Nano LCD display code
|
|-- Documentation/
|   |-- Documents/
|   |   |-- PDF project documentation
|   |
|   |-- Block diagrams/
|       |-- ALU block diagrams
|
|-- Hardware/
|   |-- components_list.md
|   |-- pinouts.md
|   |-- wiring_notes.md
|
|-- images/
|   |-- Breadboard/
|   |-- Logisim/
|   |-- TinkerCAD/
|
|-- simulation/
|   |-- Logisim circuit files
|
|-- README.md
```

---

## Documentation

The main documentation is inside:

```text
Documentation/Documents/
```

Start with:

```text
00_complete_project_guide.pdf
```

That PDF gives the full explanation of the project, including the ALU architecture, logic equations, modules, testing, hardware implementation, and Arduino display system.

The other PDFs focus on specific parts of the project.

---

## Hardware Notes

The `Hardware/` folder contains practical notes for the physical build:

- IC list
- Pinouts
- Wiring notes
- Capacitor notes
- DIP switch wiring
- Debugging notes
- Multiplier and signed correction wiring

This folder is mainly useful when rebuilding or checking the breadboard version.

---

## Simulations and Images

The project includes screenshots from:

- Logisim
- TinkerCAD
- Physical breadboard build

These are stored inside the `images/` directory.

The simulation files are stored inside:

```text
simulation/
```

Screenshots are useful for quick viewing, while the simulation files are useful for opening and testing the circuit directly.

---

## Testing

The project was tested using binary test cases for each module.

### ADD

```text
0011 + 0010 = 0101
0111 + 0001 = 1000   overflow = 1
1111 + 1111 = 1110   overflow = 0
```

### SUB

```text
0011 - 0001 = 0010
0000 - 0001 = 1111
1000 - 0001 = 0111   overflow = 1
```

### Unsigned Magnitude Multiplication

```text
0011 x 0011 = 00001001
0101 x 0011 = 00001111
1111 x 1111 = 11100001
```

### Signed Multiplication

```text
0011 x 1110 = 11111010
```

This means:

```text
+3 x -2 = -6
```

---

## Tools Used

- Logisim / Logisim Evolution
- TinkerCAD Circuits
- Physical breadboards
- 74xx logic ICs
- Arduino Nano
- I2C LCD
- 74HC165 shift registers

---

## Notes

This is not a microcontroller-based calculator.

The Arduino is only used to read and display the result. The actual ALU operations are done by physical logic gates, adders, comparator ICs, and breadboard wiring.

The project is meant to show how arithmetic and logic operations can be built from real digital hardware instead of only software or simulation.
# 4-Bit Signed ALU on Breadboards

This project is a fully functional 4-bit signed Arithmetic Logic Unit built using logic ICs on breadboards. It uses two's complement representation and supports addition, subtraction, signed multiplication, and signed comparison.

The ALU calculations are performed entirely by hardware logic ICs. An Arduino Nano is used only as a display interface: it reads the real ALU outputs through 74HC165 shift registers and displays the selected result on an I2C LCD.# 4Bit_Signed_ALU
4 bit 2's complement ALU + Arduino nano for lcd output display
