# Wiring Notes

This file contains practical wiring notes for the physical breadboard implementation of the 4-bit signed ALU.

## Power Rules

Use a stable 5V supply for the logic ICs.

All breadboards and modules must share common ground:

```text
GND rail of board 1 -> GND rail of board 2 -> GND rail of board 3
```

## Decoupling Capacitors

Place one `100nF` ceramic capacitor near every IC:

```text
VCC -- 100nF -- GND
```

Example for `74HC283`:

```text
pin 16 -- 100nF -- pin 8
```

Also place one larger capacitor across each breadboard power rail:

```text
+5V -- 47uF or 100uF -- GND
```

For electrolytic capacitors:

```text
positive leg -> +5V
negative/stripe side -> GND
```
## Capacitor Note

Capacitors are used in this circuit to make the power supply more stable and reduce noise.

When digital ICs switch between HIGH and LOW, they briefly demand extra current. This sudden current demand can cause small voltage dips or spikes on the 5V rail, especially on breadboards with long wires and many ICs. These dips can make logic gates behave incorrectly, causing random blinking, false HIGHs, or unstable outputs.

The small `100nF` decoupling capacitors act like tiny local energy reserves beside each IC. They quickly supply current during fast switching moments, helping the IC see a cleaner and more stable 5V supply.

The larger `47µF` or `100µF` capacitors across the breadboard power rails act like a bigger energy reserve for the whole board. They help smooth slower or larger voltage drops caused by multiple ICs switching or LEDs turning on.

Capacitors do not create or regulate 5V by themselves. They only help smooth and stabilize the voltage that is already supplied by the power source.

## DIP Switch Inputs

Each DIP switch input should use a pulldown resistor.

Recommended wiring:

```text
+5V ---- DIP switch ---- signal node ----> IC input
                         |
                        1kΩ
                         |
                        GND
```

Use `1kΩ or 10kΩ ` especially for:

```text
A0 A1 A2 A3
B0 B1 B2 B3
SUB
SEL0
SEL1
Cin-related control lines
```

## LED Wiring

For normal active-HIGH LED indicators:

```text
logic output -> LED -> 330Ω/1kΩ resistor -> GND
```

This means:

```text
output HIGH -> LED ON
output LOW  -> LED OFF
```

Avoid placing LEDs directly on sensitive input/control lines while debugging, especially:

```text
A3
B3
SUB
Cin
ProductSign
```

LEDs can load or backfeed weak signals.

## ADD/SUB Wiring

The ADD/SUB unit is based on:

```text
A + (B XOR SUB) + SUB
```

Where:

```text
SUB = 0 -> A + B
SUB = 1 -> A - B
```

For the `74HC283` ADD/SUB adder:

```text
A inputs = A0 A1 A2 A3
B inputs = B0 XOR SUB, B1 XOR SUB, B2 XOR SUB, B3 XOR SUB
Cin = SUB
```

Use a local pulldown at the adder carry-in:

```text
SUB node -> 74HC283 Cin
Cin -> 1kΩ -> GND
```

Do not put the `1kΩ` resistor in series. It must be a pulldown.

## Overflow Logic

Universal ADD/SUB overflow rule:

```text
B_eff3 = B3 XOR SUB
Overflow = NOT(A3 XOR B_eff3) AND (A3 XOR S3)
```

For subtraction only:

```text
Overflow = (A3 XOR B3) AND (A3 XOR S3)
```

`Cout` is not the signed overflow flag.

## Absolute Value / Two's Complementer

To convert a 4-bit two's complement number to magnitude:

```text
Magnitude = (Input XOR SignBit) + SignBit
```

If sign bit is `0`, the number stays the same.

If sign bit is `1`, the bits are inverted and `1` is added.

Use local `1kΩ` pulldowns on carry-in lines controlled by sign bits.

## Comparator Wiring

The `74HC85` first produces unsigned outputs:

```text
U_GT
U_EQ
U_LT
```

For standalone 4-bit comparison, set cascade inputs:

```text
I_GT = 0
I_EQ = 1
I_LT = 0
```

Signed correction logic:

```text
sameSign = NOT(A3 XOR B3)

GT = (NOT A3 AND B3) OR (sameSign AND U_GT)

EQ = U_EQ

LT = (A3 AND NOT B3) OR (sameSign AND U_LT)
```

## Unsigned Multiplier Wiring

The unsigned magnitude multiplier computes:

```text
Amag[3:0] x Bmag[3:0] = Pmag[7:0]
```

It uses:

```text
16 AND gates
3 partial-addition stages
```

Partial products:

```text
p00 = A0 AND B0
p10 = A1 AND B0
p20 = A2 AND B0
p30 = A3 AND B0

p01 = A0 AND B1
p11 = A1 AND B1
p21 = A2 AND B1
p31 = A3 AND B1

p02 = A0 AND B2
p12 = A1 AND B2
p22 = A2 AND B2
p32 = A3 AND B2

p03 = A0 AND B3
p13 = A1 AND B3
p23 = A2 AND B3
p33 = A3 AND B3
```

Product bit:

```text
P0 = p00
```

Adder 1:

```text
A inputs = p10, p20, p30, 0
B inputs = p01, p11, p21, p31
Cin = 0
```

Outputs:

```text
P1 = S0
T2 = S1
T3 = S2
T4 = S3
T5 = Cout
```

Adder 2:

```text
A inputs = T2, T3, T4, T5
B inputs = p02, p12, p22, p32
Cin = 0
```

Outputs:

```text
P2 = S0
U3 = S1
U4 = S2
U5 = S3
U6 = Cout
```

Adder 3:

```text
A inputs = U3, U4, U5, U6
B inputs = p03, p13, p23, p33
Cin = 0
```

Outputs:

```text
P3 = S0
P4 = S1
P5 = S2
P6 = S3
P7 = Cout
```

Important note:

```text
In the unsigned multiplier, each adder Cin is tied to GND.
The Cout of one stage becomes a normal input bit in the next stage, not the next adder's Cin.
```

## Signed Multiplier Correction

After getting the magnitude product:

```text
Pmag[7:0]
```

Compute the final product sign:

```text
ProductSign = A3 XOR B3
```

Then:

```text
FinalProduct = (Pmag XOR ProductSign) + ProductSign
```

Here, `+ ProductSign` means binary addition, not OR.

Implementation:

```text
X0 = P0 XOR ProductSign
X1 = P1 XOR ProductSign
X2 = P2 XOR ProductSign
X3 = P3 XOR ProductSign
X4 = P4 XOR ProductSign
X5 = P5 XOR ProductSign
X6 = P6 XOR ProductSign
X7 = P7 XOR ProductSign
```

Low correction adder:

```text
A inputs = X0, X1, X2, X3
B inputs = GND, GND, GND, GND
Cin = ProductSign
```

High correction adder:

```text
A inputs = X4, X5, X6, X7
B inputs = GND, GND, GND, GND
Cin = Cout from low correction adder
```

Outputs:

```text
Low adder S0-S3  = FinalProduct P0-P3
High adder S0-S3 = FinalProduct P4-P7
```

## General Debugging Rules

Measure directly on IC pins, not only on LEDs.

For HC logic:

```text
LOW should be close to 0V
HIGH should be close to 5V
```

Avoid half-values such as:

```text
0.5V
1.4V
2.5V
```

These usually mean floating inputs, weak pulldowns, backfeeding, or bad wiring.

When debugging:

```text
1. Disconnect outputs from other modules.
2. Test one IC or one block at a time.
3. Tie all unused inputs.
4. Add modules back one by one.
5. If touching/moving wires changes outputs, fix the physical connection first.
```

## Breadboard Layout Advice

Do not cram the full multiplier into the same area as the ADD/SUB and comparator.

Recommended physical split:

```text
Board 1: inputs, ADD/SUB, comparator
Board 2: absolute value / magnitude preparation
Board 3: multiplier partial products and adders
Board 4: signed product correction and final outputs
```

The multiplier has many wires, so keeping it separated makes debugging easier.
