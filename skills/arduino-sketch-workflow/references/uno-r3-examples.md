# UNO R3 Examples And Troubleshooting

## Known-good Arduino CLI setup

Use these checks before compile/upload:

```sh
arduino-cli board list
arduino-cli core list
arduino-cli lib list
```

Expected UNO R3 board row:

```text
Port         Protocol Type              Board Name  FQBN            Core
/dev/ttyACM0 serial   Serial Port (USB) Arduino UNO arduino:avr:uno arduino:avr
```

Expected AVR core row:

```text
ID          Installed Latest Name
arduino:avr 1.8.8     1.8.8  Arduino AVR Boards
```

If the core is missing:

```sh
arduino-cli core update-index
arduino-cli core install arduino:avr
```

## Blink plus serial sketch

Use this for basic UNO R3 verification. It blinks the built-in pin 13 LED and prints at 9600 baud.

```cpp
const unsigned long LED_ON_MS = 1000;
const unsigned long LED_OFF_MS = 500;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; // Harmless on Arduino UNO R3.
  }
  Serial.println("Arduino UNO R3 test sketch started");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED on");
  delay(LED_ON_MS);

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED off");
  delay(LED_OFF_MS);
}
```

Serial monitor:

```sh
arduino-cli monitor -p /dev/ttyACM0 -b arduino:avr:uno -c baudrate=9600
```

Expected output:

```text
Arduino UNO R3 test sketch started
LED on
LED off
LED on
LED off
```

## 16x2 LCD sketch with LiquidCrystal

Use `LiquidCrystal.h` for HD44780-compatible 16x2 LCDs in 4-bit parallel mode. The common Arduino Starter Kit wiring is:

```text
RS -> 12
E  -> 11
D4 -> 5
D5 -> 4
D6 -> 3
D7 -> 2
```

If the library is missing:

```sh
arduino-cli lib install LiquidCrystal
```

Example sketch:

```cpp
#include <LiquidCrystal.h>

const int LCD_RS = 12;
const int LCD_ENABLE = 11;
const int LCD_D4 = 5;
const int LCD_D5 = 4;
const int LCD_D6 = 3;
const int LCD_D7 = 2;

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void printPaddedLine(const char *text) {
  int count = 0;

  while (*text && count < LCD_COLS) {
    lcd.print(*text++);
    count++;
  }

  while (count < LCD_COLS) {
    lcd.print(' ');
    count++;
  }
}

void setup() {
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();

  lcd.setCursor(0, 0);
  printPaddedLine("Arduino UNO R3");
}

void loop() {
  char line[LCD_COLS + 1];

  snprintf(line, sizeof(line), "Uptime: %lus", millis() / 1000);
  lcd.setCursor(0, 1);
  printPaddedLine(line);

  delay(1000);
}
```

Expected LCD output:

```text
Arduino UNO R3
Uptime: 285s
```

## Troubleshooting from the working thread

- `Cannot create build directory: mkdir /home/.../.cache/arduino/...: read-only file system`: compile with `--build-path /tmp/<name>-build`.
- `fatal error: LiquidCrystal.h: No such file or directory`: run `arduino-cli lib install LiquidCrystal`, then compile again.
- `Error starting discovery ... operation not permitted`: serial/USB discovery needs host access approval; rerun `arduino-cli board list` outside the sandbox if available.
- `No boards found`: check the USB cable, board power LED, and `/dev/ttyACM0`; then rerun `arduino-cli board list`.
- Upload success may print only `New upload port: /dev/ttyACM0 (serial)`.
- A serial monitor may show a partial first line when opened during board reset; clean repeated lines after that are enough to confirm operation.
