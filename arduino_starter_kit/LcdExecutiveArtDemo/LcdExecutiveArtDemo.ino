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

const char BRAND_NAME[] = "CODEX";
const char BRAND_TAGLINE[] = "AGENT ONLINE";
const char BRAND_MARK_NAME[] = "OpenAI Codex";

const unsigned long FRAME_INTERVAL_MS = 140;
const unsigned long SIGNAL_SCENE_MS = 5200;
const unsigned long WAVE_SCENE_MS = 5200;
const unsigned long PRECISION_SCENE_MS = 5400;
const unsigned long FINALE_SCENE_MS = 5600;

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

enum Scene {
  SCENE_SIGNAL,
  SCENE_WAVE,
  SCENE_PRECISION,
  SCENE_FINALE
};

Scene currentScene = SCENE_SIGNAL;
unsigned long sceneStartedAtMs = 0;
unsigned long lastFrameAtMs = 0;
unsigned int frame = 0;

void writePaddedLine(byte row, const char *text) {
  byte col = 0;

  lcd.setCursor(0, row);
  while (text[col] && col < LCD_COLS) {
    lcd.print(text[col]);
    col++;
  }

  while (col < LCD_COLS) {
    lcd.print(' ');
    col++;
  }
}

void writeCentered(byte row, const char *text) {
  byte length = strlen(text);
  byte leftPadding = length < LCD_COLS ? (LCD_COLS - length) / 2 : 0;

  lcd.setCursor(0, row);
  for (byte col = 0; col < leftPadding; col++) {
    lcd.print(' ');
  }

  for (byte col = 0; col < length && col + leftPadding < LCD_COLS; col++) {
    lcd.print(text[col]);
  }

  for (byte col = leftPadding + length; col < LCD_COLS; col++) {
    lcd.print(' ');
  }
}

void writeBrandedNameLine(byte row) {
  const byte markWidth = 4;
  byte nameLength = strlen(BRAND_NAME);
  byte innerWidth = LCD_COLS - markWidth;
  byte leftPadding = nameLength < innerWidth ? (innerWidth - nameLength) / 2 : 0;

  lcd.setCursor(0, row);
  lcd.write((byte)0);
  lcd.write((byte)1);
  lcd.write((byte)2);
  lcd.print(' ');

  for (byte col = 0; col < innerWidth; col++) {
    if (col >= leftPadding && col < leftPadding + nameLength) {
      lcd.print(BRAND_NAME[col - leftPadding]);
    } else {
      lcd.print(' ');
    }
  }
}

void loadSignalGlyphs() {
  byte glyphs[8][8] = {
    {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000},
    {B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111},
    {B00000, B00000, B00000, B00000, B11111, B11111, B11111, B11111},
    {B00000, B00000, B11111, B11111, B11111, B11111, B11111, B11111},
    {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111},
    {B00100, B01110, B11111, B00100, B00100, B11111, B01110, B00100},
    {B00000, B00100, B01110, B11111, B01110, B00100, B00000, B00000},
    {B10001, B01010, B00100, B11111, B00100, B01010, B10001, B00000}
  };

  for (byte index = 0; index < 8; index++) {
    lcd.createChar(index, glyphs[index]);
  }
}

void loadWaveGlyphs() {
  byte glyphs[8][8] = {
    {B00000, B00000, B10000, B01000, B00100, B00010, B00001, B00000},
    {B00000, B00001, B00010, B00100, B01000, B10000, B00000, B00000},
    {B00100, B01010, B10001, B00000, B00000, B10001, B01010, B00100},
    {B00000, B00100, B01110, B11111, B01110, B00100, B00000, B00000},
    {B00000, B00100, B00100, B11111, B00100, B00100, B00000, B00000},
    {B00100, B00100, B01110, B01110, B11111, B11111, B00000, B00000},
    {B00000, B00000, B00100, B01010, B10001, B01010, B00100, B00000},
    {B11111, B00000, B11111, B00000, B11111, B00000, B11111, B00000}
  };

  for (byte index = 0; index < 8; index++) {
    lcd.createChar(index, glyphs[index]);
  }
}

void loadPrecisionGlyphs() {
  byte glyphs[8][8] = {
    {B00100, B00100, B11111, B00100, B00100, B00000, B00000, B00000},
    {B00000, B00100, B01110, B11111, B01110, B00100, B00000, B00000},
    {B10001, B01010, B00100, B11111, B00100, B01010, B10001, B00000},
    {B11111, B10001, B10101, B10001, B10101, B10001, B11111, B00000},
    {B00100, B01110, B10101, B00100, B10101, B01110, B00100, B00000},
    {B00000, B00000, B00100, B01110, B11111, B01110, B00100, B00000},
    {B00000, B00100, B00000, B10001, B01110, B10001, B00000, B00100},
    {B11111, B10001, B10001, B10001, B10001, B10001, B11111, B00000}
  };

  for (byte index = 0; index < 8; index++) {
    lcd.createChar(index, glyphs[index]);
  }
}

void loadBrandGlyphs() {
  byte glyphs[8][8] = {
    {B01110, B11111, B11111, B11111, B11111, B11111, B01110, B00000},
    {B11111, B11101, B11011, B10111, B11011, B11101, B11111, B00000},
    {B01110, B11111, B11111, B11111, B11111, B11111, B01110, B00000},
    {B00000, B11000, B01100, B00110, B01100, B11000, B00000, B00000},
    {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111},
    {B00000, B00100, B01110, B11111, B01110, B00100, B00000, B00000},
    {B11111, B10000, B10000, B10000, B10000, B10000, B10000, B11111},
    {B11111, B00001, B00001, B00001, B00001, B00001, B00001, B11111}
  };

  for (byte index = 0; index < 8; index++) {
    lcd.createChar(index, glyphs[index]);
  }
}

void showWakeSequence() {
  loadBrandGlyphs();
  lcd.clear();

  for (byte col = 0; col < LCD_COLS; col++) {
    lcd.setCursor(col, 0);
    lcd.write((byte)5);
    lcd.setCursor(LCD_COLS - 1 - col, 1);
    lcd.write((byte)5);
    delay(35);
  }

  delay(120);
  lcd.clear();

  for (byte step = 0; step <= 14; step++) {
    writeBrandedNameLine(0);

    lcd.setCursor(0, 1);
    lcd.print('[');
    for (byte col = 0; col < 12; col++) {
      lcd.write(col < step ? (byte)4 : ' ');
    }
    lcd.print(']');
    lcd.write(step % 2 == 0 ? (byte)3 : (byte)5);
    lcd.print(' ');
    delay(90);
  }

  writeBrandedNameLine(0);
  writeCentered(1, BRAND_TAGLINE);
  delay(220);
}

void drawSignalScene() {
  writePaddedLine(0, "LIVE SIGNAL");

  lcd.setCursor(12, 0);
  lcd.write((byte)(5 + (frame % 3)));

  lcd.setCursor(0, 1);
  for (byte col = 0; col < LCD_COLS; col++) {
    byte height = ((col * 3) + frame + (col % 4)) % 5;
    lcd.write(height == 0 ? (byte)1 : height);
  }
}

void drawWaveScene() {
  const char spinner[] = {'-', '\\', '|', '/'};

  writePaddedLine(0, "SYSTEM READY");
  lcd.setCursor(14, 0);
  lcd.write((byte)3);
  lcd.print(spinner[frame % 4]);

  lcd.setCursor(0, 1);
  for (byte col = 0; col < LCD_COLS; col++) {
    byte phase = (col + frame) % 8;
    if (phase == 0 || phase == 4) {
      lcd.write((byte)2);
    } else if (phase < 4) {
      lcd.write((byte)0);
    } else {
      lcd.write((byte)1);
    }
  }
}

void drawPrecisionScene() {
  const byte revealLength = min((unsigned int)LCD_COLS, frame / 2);
  const char title[] = "PRECISION MODE";

  lcd.setCursor(0, 0);
  for (byte col = 0; col < LCD_COLS; col++) {
    if (col < revealLength && col < strlen(title)) {
      lcd.print(title[col]);
    } else if ((col + frame) % 5 == 0) {
      lcd.write((byte)1);
    } else {
      lcd.print(' ');
    }
  }

  lcd.setCursor(0, 1);
  for (byte col = 0; col < LCD_COLS; col++) {
    byte pattern = (col * 7 + frame) % 16;
    if (pattern == 0) {
      lcd.write((byte)2);
    } else if (pattern < 3) {
      lcd.write((byte)0);
    } else if (pattern == 8) {
      lcd.write((byte)4);
    } else if (pattern > 12) {
      lcd.write((byte)7);
    } else {
      lcd.print(' ');
    }
  }
}

void drawFinaleScene() {
  byte sweep = frame % LCD_COLS;

  lcd.setCursor(0, 0);
  lcd.write((byte)6);
  for (byte col = 1; col < LCD_COLS - 1; col++) {
    if (col == sweep || col == LCD_COLS - 1 - sweep) {
      lcd.write((byte)5);
    } else {
      lcd.print('-');
    }
  }
  lcd.write((byte)7);

  writeBrandedNameLine(1);
}

unsigned long currentSceneDuration() {
  switch (currentScene) {
    case SCENE_SIGNAL:
      return SIGNAL_SCENE_MS;
    case SCENE_WAVE:
      return WAVE_SCENE_MS;
    case SCENE_PRECISION:
      return PRECISION_SCENE_MS;
    case SCENE_FINALE:
      return FINALE_SCENE_MS;
  }

  return SIGNAL_SCENE_MS;
}

void startScene(Scene scene) {
  currentScene = scene;
  sceneStartedAtMs = millis();
  lastFrameAtMs = 0;
  frame = 0;
  lcd.clear();

  switch (currentScene) {
    case SCENE_SIGNAL:
      loadSignalGlyphs();
      Serial.println("Scene: live signal");
      break;
    case SCENE_WAVE:
      loadWaveGlyphs();
      Serial.println("Scene: system ready");
      break;
    case SCENE_PRECISION:
      loadPrecisionGlyphs();
      Serial.println("Scene: precision mode");
      break;
    case SCENE_FINALE:
      loadBrandGlyphs();
      Serial.print("Scene: finale brand=");
      Serial.print(BRAND_NAME);
      Serial.print(" mark=");
      Serial.println(BRAND_MARK_NAME);
      break;
  }
}

void drawCurrentScene() {
  switch (currentScene) {
    case SCENE_SIGNAL:
      drawSignalScene();
      break;
    case SCENE_WAVE:
      drawWaveScene();
      break;
    case SCENE_PRECISION:
      drawPrecisionScene();
      break;
    case SCENE_FINALE:
      drawFinaleScene();
      break;
  }
}

Scene nextScene() {
  switch (currentScene) {
    case SCENE_SIGNAL:
      return SCENE_WAVE;
    case SCENE_WAVE:
      return SCENE_PRECISION;
    case SCENE_PRECISION:
      return SCENE_FINALE;
    case SCENE_FINALE:
      return SCENE_SIGNAL;
  }

  return SCENE_SIGNAL;
}

void setup() {
  Serial.begin(LOG_BAUD_RATE);
  Serial.print("LCD executive art demo starting for ");
  Serial.print(BRAND_NAME);
  Serial.print(" with symbolic ");
  Serial.print(BRAND_MARK_NAME);
  Serial.println(" mark");

  lcd.begin(LCD_COLS, LCD_ROWS);
  showWakeSequence();
  startScene(SCENE_SIGNAL);
}

void loop() {
  unsigned long now = millis();

  if (now - sceneStartedAtMs >= currentSceneDuration()) {
    startScene(nextScene());
  }

  if (now - lastFrameAtMs >= FRAME_INTERVAL_MS) {
    lastFrameAtMs = now;
    drawCurrentScene();
    frame++;
  }
}
