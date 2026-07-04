# Pinouts

This file contains important IC pinouts used in the 4-bit signed ALU project.

## 74HC283 / 74LS283 4-Bit Binary Adder

```text
        74HC283 / 74LS283
      +---U---+
 S1  |1     16| VCC
 B1  |2     15| B2
 A1  |3     14| A2
 S0  |4     13| S2
 A0  |5     12| A3
 B0  |6     11| B3
 C0  |7     10| S3
 GND |8      9| C4
      +-------+
```

### Bit Mapping

| Signal | Pin |
|---|---:|
| `A0` | 5 |
| `B0` | 6 |
| `S0` | 4 |
| `A1` | 3 |
| `B1` | 2 |
| `S1` | 1 |
| `A2` | 14 |
| `B2` | 15 |
| `S2` | 13 |
| `A3` | 12 |
| `B3` | 11 |
| `S3` | 10 |
| `Cin / C0` | 7 |
| `Cout / C4` | 9 |
| `VCC` | 16 |
| `GND` | 8 |

---

## 74HC86 Quad XOR Gate

Used for conditional inversion, SUB logic, and sign correction.

```text
        74HC86
      +---U---+
 1A  |1     14| VCC
 1B  |2     13| 4B
 1Y  |3     12| 4A
 2A  |4     11| 4Y
 2B  |5     10| 3B
 2Y  |6      9| 3A
 GND |7      8| 3Y
      +-------+
```

| Gate | Inputs | Output |
|---|---|---|
| Gate 1 | pins 1, 2 | pin 3 |
| Gate 2 | pins 4, 5 | pin 6 |
| Gate 3 | pins 9, 10 | pin 8 |
| Gate 4 | pins 12, 13 | pin 11 |

---

## 74HC08 Quad AND Gate

Used for multiplier partial products and comparator correction.

```text
        74HC08
      +---U---+
 1A  |1     14| VCC
 1B  |2     13| 4B
 1Y  |3     12| 4A
 2A  |4     11| 4Y
 2B  |5     10| 3B
 2Y  |6      9| 3A
 GND |7      8| 3Y
      +-------+
```

| Gate | Inputs | Output |
|---|---|---|
| Gate 1 | pins 1, 2 | pin 3 |
| Gate 2 | pins 4, 5 | pin 6 |
| Gate 3 | pins 9, 10 | pin 8 |
| Gate 4 | pins 12, 13 | pin 11 |

---

## 74HC32 Quad OR Gate

Used in comparator signed-correction logic.

```text
        74HC32
      +---U---+
 1A  |1     14| VCC
 1B  |2     13| 4B
 1Y  |3     12| 4A
 2A  |4     11| 4Y
 2B  |5     10| 3B
 2Y  |6      9| 3A
 GND |7      8| 3Y
      +-------+
```

| Gate | Inputs | Output |
|---|---|---|
| Gate 1 | pins 1, 2 | pin 3 |
| Gate 2 | pins 4, 5 | pin 6 |
| Gate 3 | pins 9, 10 | pin 8 |
| Gate 4 | pins 12, 13 | pin 11 |

---

## 74HC04 Hex NOT Gate

Used to invert sign bits and XOR results.

```text
        74HC04
      +---U---+
 1A  |1     14| VCC
 1Y  |2     13| 6A
 2A  |3     12| 6Y
 2Y  |4     11| 5A
 3A  |5     10| 5Y
 3Y  |6      9| 4A
 GND |7      8| 4Y
      +-------+
```

| Inverter | Input | Output |
|---|---:|---:|
| 1 | 1 | 2 |
| 2 | 3 | 4 |
| 3 | 5 | 6 |
| 4 | 9 | 8 |
| 5 | 11 | 10 |
| 6 | 13 | 12 |

---

## 74HC85 / 74LS85 4-Bit Comparator

Used to compare `A[3:0]` and `B[3:0]` as unsigned values first.

The unsigned outputs are:

```text
U_GT
U_EQ
U_LT
```

For standalone 4-bit comparator operation, the cascade inputs should be:

```text
I_GT = 0
I_EQ = 1
I_LT = 0
```

This tells the comparator that the previous comparison stage is equal.

Final signed correction is performed outside the comparator IC.

---

## General HC Logic Rules

- `VCC` must connect to `+5V`.
- `GND` must connect to ground.
- Every unused input must be tied to either `GND` or `+5V`.
- Unused outputs can be left unconnected.
- Add a `100nF` capacitor between `VCC` and `GND` for every IC.
