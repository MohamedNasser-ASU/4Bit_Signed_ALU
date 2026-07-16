/*
  4-Bit Signed ALU Display
  ------------------------
  Hardware:
  - Arduino Nano (classic ATmega328P)
  - 3 x 74HC165 parallel-in/serial-out shift registers
  - 16x2 HD44780 LCD with I2C backpack
  - hd44780 library by Bill Perry

  Shift-register chain:
    Register 1 QH (pin 9) -> Register 2 SER (pin 10)
    Register 2 QH (pin 9) -> Register 3 SER (pin 10)
    Register 3 QH (pin 9) -> Nano D2

  Shared control:
    Nano D3 -> CLK (pin 2) of all three 74HC165s
    Nano D4 -> SH/LD (pin 1) of all three 74HC165s

  Parallel-input assignments:
    Register 1:
      A=P0, B=P1, C=P2, D=P3, E=P4, F=P5, G=P6, H=P7

    Register 2:
      A=S0, B=S1, C=S2, D=S3, E=Overflow, F=GT, G=EQ, H=LT

    Register 3:
      A=SEL0, B=SEL1, C=SUB, D-H=GND

  Selection:
    SEL1 SEL0 = 00 -> Addition      (SUB must be 0)
    SEL1 SEL0 = 01 -> Subtraction   (SUB must be 1)
    SEL1 SEL0 = 10 -> Comparison
    SEL1 SEL0 = 11 -> Multiplication
*/

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <string.h>

hd44780_I2Cexp lcd;

constexpr uint8_t DATA_PIN  = 2;
constexpr uint8_t CLOCK_PIN = 3;
constexpr uint8_t LOAD_PIN  = 4;

constexpr uint8_t LCD_COLUMNS = 16;
constexpr uint8_t LCD_ROWS    = 2;
constexpr uint8_t TOTAL_BITS  = 24;

char previousLine1[17] = "";
char previousLine2[17] = "";

void readShiftRegisters(bool bits[TOTAL_BITS]) {
  // Capture A-H on all three 74HC165s.
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LOAD_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(LOAD_PIN, HIGH);
  delayMicroseconds(2);

  // After SH/LD returns HIGH, QH already contains the first bit (H).
  // Read that bit first, then pulse the clock to shift the next bit to QH.
  for (uint8_t i = 0; i < TOTAL_BITS; ++i) {
    bits[i] = digitalRead(DATA_PIN);
    digitalWrite(CLOCK_PIN, HIGH);
    delayMicroseconds(2);
    digitalWrite(CLOCK_PIN, LOW);
    delayMicroseconds(2);
  }
}

int8_t signed4(uint8_t value) {
  value &= 0x0F;
  return (value & 0x08) ? static_cast<int8_t>(value) - 16
                        : static_cast<int8_t>(value);
}

int16_t signed8(uint8_t value) {
  return (value & 0x80) ? static_cast<int16_t>(value) - 256
                        : static_cast<int16_t>(value);
}

void padAndPrint(uint8_t row, const char *text) {
  char padded[17];
  snprintf(padded, sizeof(padded), "%-16.16s", text);
  lcd.setCursor(0, row);
  lcd.print(padded);
}

void updateDisplay(const char *line1, const char *line2) {
  if (strcmp(line1, previousLine1) == 0 &&
      strcmp(line2, previousLine2) == 0) {
    return;
  }

  padAndPrint(0, line1);
  padAndPrint(1, line2);

  strncpy(previousLine1, line1, sizeof(previousLine1) - 1);
  previousLine1[sizeof(previousLine1) - 1] = '\0';

  strncpy(previousLine2, line2, sizeof(previousLine2) - 1);
  previousLine2[sizeof(previousLine2) - 1] = '\0';

  Serial.print(line1);
  Serial.print(" | ");
  Serial.println(line2);
}

void setup() {
  pinMode(DATA_PIN, INPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LOAD_PIN, OUTPUT);

  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LOAD_PIN, HIGH);

  Serial.begin(115200);

  const int lcdStatus = lcd.begin(LCD_COLUMNS, LCD_ROWS);
  if (lcdStatus != 0) {
    Serial.print("LCD initialization failed. Status: ");
    Serial.println(lcdStatus);
  }

  lcd.clear();
  updateDisplay("4-bit signed ALU", "Display starting");
  delay(1200);
}

void loop() {
  bool bits[TOTAL_BITS];
  readShiftRegisters(bits);

  const bool sub  = bits[5];
  const bool sel1 = bits[6];
  const bool sel0 = bits[7];
  const uint8_t selection = (static_cast<uint8_t>(sel1) << 1) |
                             static_cast<uint8_t>(sel0);

  const bool lessThan    = bits[8];
  const bool equalTo     = bits[9];
  const bool greaterThan = bits[10];
  const bool overflow    = bits[11];

  const uint8_t sumBits =
      (static_cast<uint8_t>(bits[12]) << 3) |
      (static_cast<uint8_t>(bits[13]) << 2) |
      (static_cast<uint8_t>(bits[14]) << 1) |
       static_cast<uint8_t>(bits[15]);

  const uint8_t productBits =
      (static_cast<uint8_t>(bits[16]) << 7) |
      (static_cast<uint8_t>(bits[17]) << 6) |
      (static_cast<uint8_t>(bits[18]) << 5) |
      (static_cast<uint8_t>(bits[19]) << 4) |
      (static_cast<uint8_t>(bits[20]) << 3) |
      (static_cast<uint8_t>(bits[21]) << 2) |
      (static_cast<uint8_t>(bits[22]) << 1) |
       static_cast<uint8_t>(bits[23]);

  const int8_t addSubResult = signed4(sumBits);
  const int16_t multiplyResult = signed8(productBits);

  char line1[17] = "";
  char line2[17] = "";

  switch (selection) {
    case 0b00:
      if (sub) {
        snprintf(line1, sizeof(line1), "Addition mode");
        snprintf(line2, sizeof(line2), "Set SUB to 0");
      } else {
        snprintf(line1, sizeof(line1), "Addition result");
        if (overflow) {
          snprintf(line2, sizeof(line2), "= %d OVERFLOW", addSubResult);
        } else {
          snprintf(line2, sizeof(line2), "= %d", addSubResult);
        }
      }
      break;

    case 0b01:
      if (!sub) {
        snprintf(line1, sizeof(line1), "Subtract mode");
        snprintf(line2, sizeof(line2), "Set SUB to 1");
      } else {
        snprintf(line1, sizeof(line1), "Subtract result");
        if (overflow) {
          snprintf(line2, sizeof(line2), "= %d OVERFLOW", addSubResult);
        } else {
          snprintf(line2, sizeof(line2), "= %d", addSubResult);
        }
      }
      break;

    case 0b10:
      snprintf(line1, sizeof(line1), "Comparison");
      if (greaterThan && !equalTo && !lessThan) {
        snprintf(line2, sizeof(line2), "A is greater");
      } else if (equalTo && !greaterThan && !lessThan) {
        snprintf(line2, sizeof(line2), "A equals B");
      } else if (lessThan && !greaterThan && !equalTo) {
        snprintf(line2, sizeof(line2), "A is less");
      } else {
        snprintf(line2, sizeof(line2), "Invalid outputs");
      }
      break;

    case 0b11:
      snprintf(line1, sizeof(line1), "Multiply result");
      snprintf(line2, sizeof(line2), "= %d", multiplyResult);
      break;
  }

  updateDisplay(line1, line2);
  delay(100);
}
