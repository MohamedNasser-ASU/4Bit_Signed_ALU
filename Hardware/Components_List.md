# Components List

This file lists the main components used in the 4-bit signed ALU project.

## Logic ICs

| Component | Quantity | Purpose |
|---|---:|---|
| `74HC283` | 8 | 4-bit binary adders used for ADD/SUB, two's complementing, multiplier partial additions, and signed product correction |
| `74HC86` | 7 | XOR gates used for controlled inversion, SUB logic, sign comparison, and signed product correction |
| `74HC08` | 5 | AND gates used for multiplier partial products and comparator correction logic |
| `74HC32` | 2 | OR gates used in comparator signed-correction logic |
| `74HC04` | 2 | NOT gates used for sign correction and inverted sign signals |
| `74HC85` | 1 | 4-bit magnitude comparator used to generate unsigned `U_GT`, `U_EQ`, and `U_LT` |

## Control and Input Components

| Component | Purpose |
|---|---|
| DIP switches | Used to enter 4-bit `A`, 4-bit `B`, `SUB`, and selection/control bits |
| Breadboards | Used for physical IC-based circuit construction |
| Jumper wires | Used for signal, power, and ground wiring |

## Display Components

| Component | Purpose |
|---|---|
| LEDs | Used to display output bits and status flags |
| 1kΩ resistors | LED current-limiting resistors |
| LCD1602 I2C |  output display using Arduino |
|  Arduino Nano | display/readout controller only, not used for ALU computation |

## Resistors

| Value | Purpose |
|---|---|
| `1kΩ` | Strong pulldown resistors for DIP switch input lines, especially sign bits and carry/sign control lines |
| `10kΩ` | Standard pulldown value, usable in cleaner parts of the circuit |
| `330Ω` | LED current limiting |

## Capacitors

| Value | Purpose |
|---|---|
| `100nF` ceramic | Decoupling capacitor placed across VCC and GND of each logic IC |
| `47µF` / `100µF` electrolytic | Bulk capacitor across main breadboard power rails to reduce voltage dips |

## Power

| Component | Purpose |
|---|---|
| 5V breadboard power module | Powers the logic ICs |
| 5V USB supply | Main power source |
