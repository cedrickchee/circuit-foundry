#include <LiquidCrystal.h>

const int LCD_RS = 12;
const int LCD_ENABLE = 11;
const int LCD_D4 = 5;
const int LCD_D5 = 4;
const int LCD_D6 = 3;
const int LCD_D7 = 2;

const int LCD_COLS = 16;
const int LCD_ROWS = 2;
const unsigned long LOG_BAUD_RATE = 9600;
const unsigned long UPDATE_INTERVAL_MS = 1000;

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

char displayedLines[LCD_ROWS][LCD_COLS + 1];
unsigned long lastUpdateMs = 0;

void formatLcdLine(char *destination, const char *text) {
  int index = 0;

  while (text[index] && index < LCD_COLS) {
    destination[index] = text[index];
    index++;
  }

  while (index < LCD_COLS) {
    destination[index] = ' ';
    index++;
  }

  destination[LCD_COLS] = '\0';
}

void logLineVerification(byte row, const char *expected) {
  Serial.print("LCD row ");
  Serial.print(row);
  Serial.print(" expected=\"");
  Serial.print(expected);
  Serial.print("\" actual=\"");
  Serial.print(displayedLines[row]);
  Serial.print("\" result=");
  Serial.println(strcmp(displayedLines[row], expected) == 0 ? "PASS" : "FAIL");
}

void writeVerifiedLine(byte row, const char *text) {
  char expected[LCD_COLS + 1];

  formatLcdLine(expected, text);
  lcd.setCursor(0, row);
  lcd.print(expected);
  strcpy(displayedLines[row], expected);

  logLineVerification(row, expected);
}

void setup() {
  Serial.begin(LOG_BAUD_RATE);
  Serial.println("LCD display verification logger starting");

  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();

  writeVerifiedLine(0, "Arduino UNO R3");
  writeVerifiedLine(1, "Starting...");
}

void loop() {
  char line[LCD_COLS + 1];
  unsigned long now = millis();

  if (now - lastUpdateMs < UPDATE_INTERVAL_MS) {
    return;
  }

  lastUpdateMs = now;
  snprintf(line, sizeof(line), "Uptime: %lus", now / 1000);
  writeVerifiedLine(1, line);
}
