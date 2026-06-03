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
const unsigned long FRAME_INTERVAL_MS = 120;
const unsigned long SCENE_DURATION_MS = 4200;

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

enum Scene {
  SCENE_BUILD_THINGS,
  SCENE_AI_PILLED,
  SCENE_BUILDMAXXING,
  SCENE_SOFTWARE_FACTORY,
  SCENE_TOKENMAXXING
};

Scene currentScene = SCENE_BUILD_THINGS;
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

void loadBuildGlyphs() {
  byte glyphs[8][8] = {
    {B00100, B00100, B11111, B00100, B00100, B00000, B00000, B00000},
    {B00000, B01110, B11111, B10101, B11111, B01110, B00000, B00000},
    {B00000, B00000, B11111, B10001, B11111, B00000, B00000, B00000},
    {B00000, B00100, B01110, B11111, B01110, B00100, B00000, B00000},
    {B11111, B10000, B10000, B10000, B10000, B10000, B10000, B11111},
    {B11111, B00001, B00001, B00001, B00001, B00001, B00001, B11111},
    {B00000, B10000, B11000, B11100, B11110, B11111, B00000, B00000},
    {B00000, B00001, B00011, B00111, B01111, B11111, B00000, B00000}
  };

  for (byte index = 0; index < 8; index++) {
    lcd.createChar(index, glyphs[index]);
  }
}

void loadAiGlyphs() {
  byte glyphs[8][8] = {
    {B01110, B10001, B10101, B10001, B10101, B10001, B01110, B00000},
    {B00000, B00100, B01110, B11111, B01110, B00100, B00000, B00000},
    {B00100, B01110, B10101, B00100, B10101, B01110, B00100, B00000},
    {B00000, B00100, B00000, B01110, B11111, B01110, B00000, B00100},
    {B11111, B10000, B10111, B10101, B10111, B10000, B11111, B00000},
    {B11111, B00001, B11101, B10101, B11101, B00001, B11111, B00000},
    {B00000, B01110, B10001, B10001, B10001, B01110, B00000, B00000},
    {B00000, B00000, B00100, B01010, B10001, B01010, B00100, B00000}
  };

  for (byte index = 0; index < 8; index++) {
    lcd.createChar(index, glyphs[index]);
  }
}

void loadBuildmaxGlyphs() {
  byte glyphs[8][8] = {
    {B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111},
    {B00000, B00000, B00000, B00000, B11111, B11111, B11111, B11111},
    {B00000, B00000, B11111, B11111, B11111, B11111, B11111, B11111},
    {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111},
    {B10000, B01000, B00100, B00010, B00001, B00000, B00000, B00000},
    {B00001, B00010, B00100, B01000, B10000, B00000, B00000, B00000},
    {B00100, B01110, B11111, B00100, B00100, B11111, B01110, B00100},
    {B11111, B10101, B11111, B10101, B11111, B10101, B11111, B00000}
  };

  for (byte index = 0; index < 8; index++) {
    lcd.createChar(index, glyphs[index]);
  }
}

void loadFactoryGlyphs() {
  byte glyphs[8][8] = {
    {B11111, B10000, B10111, B10101, B10111, B10000, B11111, B00000},
    {B11111, B00001, B11101, B10101, B11101, B00001, B11111, B00000},
    {B00000, B11111, B10001, B10101, B10101, B10001, B11111, B00000},
    {B00100, B01110, B11111, B11111, B01110, B00100, B00100, B00000},
    {B11111, B00100, B00100, B11111, B00100, B00100, B11111, B00000},
    {B00000, B00100, B01010, B10001, B01010, B00100, B00000, B00000},
    {B11111, B10101, B10101, B10101, B10101, B10101, B11111, B00000},
    {B00000, B00100, B01110, B11111, B01110, B00100, B00000, B00000}
  };

  for (byte index = 0; index < 8; index++) {
    lcd.createChar(index, glyphs[index]);
  }
}

void loadTokenGlyphs() {
  byte glyphs[8][8] = {
    {B01110, B10001, B10101, B10101, B10101, B10001, B01110, B00000},
    {B01110, B11111, B11011, B10101, B11011, B11111, B01110, B00000},
    {B00000, B00100, B01110, B11111, B01110, B00100, B00000, B00000},
    {B11111, B10000, B10000, B10000, B10000, B10000, B10000, B11111},
    {B11111, B00001, B00001, B00001, B00001, B00001, B00001, B11111},
    {B10101, B01010, B10101, B01010, B10101, B01010, B10101, B01010},
    {B00000, B10000, B01000, B00100, B00010, B00001, B00000, B00000},
    {B00000, B00001, B00010, B00100, B01000, B10000, B00000, B00000}
  };

  for (byte index = 0; index < 8; index++) {
    lcd.createChar(index, glyphs[index]);
  }
}

void showBootAnimation() {
  loadBuildGlyphs();
  lcd.clear();

  for (byte col = 0; col < LCD_COLS; col++) {
    lcd.setCursor(col, 0);
    lcd.write((byte)3);
    lcd.setCursor(LCD_COLS - 1 - col, 1);
    lcd.write((byte)3);
    delay(30);
  }

  delay(120);
  lcd.clear();

  for (byte step = 0; step <= 12; step++) {
    writeCentered(0, "BUILDER HOOKS");
    lcd.setCursor(1, 1);
    lcd.print('[');
    for (byte col = 0; col < 12; col++) {
      lcd.write(col < step ? (byte)1 : ' ');
    }
    lcd.print(']');
    delay(70);
  }

  delay(180);
}

void drawBuildThingsScene() {
  writeCentered(0, "BUILD THINGS");

  lcd.setCursor(0, 1);
  lcd.write((byte)4);
  for (byte col = 1; col < LCD_COLS - 1; col++) {
    byte phase = (col + frame) % 6;
    if (phase == 0) {
      lcd.write((byte)3);
    } else if (phase == 1 || phase == 4) {
      lcd.write((byte)1);
    } else if (phase == 2) {
      lcd.write((byte)2);
    } else {
      lcd.print('-');
    }
  }
  lcd.write((byte)5);
}

void drawAiPilledScene() {
  const char *beat = ((frame / 12) % 2 == 0) ? "LOSING SLEEP" : "POSSIBILITIES";
  byte pulse = frame % 4;

  lcd.setCursor(0, 0);
  lcd.write((byte)(pulse == 0 ? 1 : 6));
  lcd.print(" AI PILLED ");
  lcd.write((byte)(pulse == 2 ? 1 : 7));
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.write((byte)4);
  byte leftPadding = (LCD_COLS - 2 - strlen(beat)) / 2;
  for (byte col = 0; col < LCD_COLS - 2; col++) {
    if (col >= leftPadding && col < leftPadding + strlen(beat)) {
      lcd.print(beat[col - leftPadding]);
    } else if ((col + frame) % 7 == 0) {
      lcd.write((byte)3);
    } else {
      lcd.print(' ');
    }
  }
  lcd.write((byte)5);

  if ((frame / 24) % 2 == 1) {
    writeCentered(0, "AUTOMATION");
  }
}

void drawBuildmaxScene() {
  writeCentered(0, "BUILDMAXXING");

  lcd.setCursor(0, 1);
  for (byte col = 0; col < LCD_COLS; col++) {
    byte pattern = (col * 3 + frame) % 12;
    if (pattern < 4) {
      lcd.write((byte)pattern);
    } else if (pattern == 4 || pattern == 8) {
      lcd.write((byte)6);
    } else if (pattern == 5) {
      lcd.write((byte)4);
    } else if (pattern == 9) {
      lcd.write((byte)5);
    } else {
      lcd.print(' ');
    }
  }
}

void drawFactoryScene() {
  if ((frame / 14) % 3 == 0) {
    writePaddedLine(0, "SOFTWARE FACTORY");
  } else if ((frame / 14) % 3 == 1) {
    writePaddedLine(0, "MICHELIN KITCHEN");
  } else {
    writeCentered(0, "ANTI SLOP");
  }

  lcd.setCursor(0, 1);
  lcd.write((byte)0);
  for (byte col = 1; col < LCD_COLS - 1; col++) {
    byte pass = (frame + col) % 10;
    if (pass == 0) {
      lcd.write((byte)7);
    } else if (pass == 4) {
      lcd.write((byte)3);
    } else if (pass == 6) {
      lcd.write((byte)5);
    } else {
      lcd.write((byte)2);
    }
  }
  lcd.write((byte)1);
}

void drawTokenmaxScene() {
  char counter[17];
  unsigned int tokens = (frame * 73U) % 10000U;

  snprintf(counter, sizeof(counter), "TOKENS %04u", tokens);
  writeCentered(0, "TOKENMAXXING");

  lcd.setCursor(0, 1);
  lcd.write((byte)3);
  byte leftPadding = (LCD_COLS - 2 - strlen(counter)) / 2;
  for (byte col = 0; col < LCD_COLS - 2; col++) {
    if (col == ((frame / 2) % (LCD_COLS - 2))) {
      lcd.write((byte)2);
    } else if (col >= leftPadding && col < leftPadding + strlen(counter)) {
      lcd.print(counter[col - leftPadding]);
    } else if ((col + frame) % 5 == 0) {
      lcd.write((byte)((frame / 4) % 2));
    } else {
      lcd.print(' ');
    }
  }
  lcd.write((byte)4);
}

void startScene(Scene scene) {
  currentScene = scene;
  sceneStartedAtMs = millis();
  lastFrameAtMs = 0;
  frame = 0;

  switch (currentScene) {
    case SCENE_BUILD_THINGS:
      loadBuildGlyphs();
      Serial.println(F("Scene 1: BUILD THINGS"));
      break;
    case SCENE_AI_PILLED:
      loadAiGlyphs();
      Serial.println(F("Scene 2: AI PILLED"));
      break;
    case SCENE_BUILDMAXXING:
      loadBuildmaxGlyphs();
      Serial.println(F("Scene 3: BUILDMAXXING"));
      break;
    case SCENE_SOFTWARE_FACTORY:
      loadFactoryGlyphs();
      Serial.println(F("Scene 4: SOFTWARE FACTORY"));
      break;
    case SCENE_TOKENMAXXING:
      loadTokenGlyphs();
      Serial.println(F("Scene 5: TOKENMAXXING"));
      break;
  }

  lcd.clear();
}

Scene nextScene() {
  switch (currentScene) {
    case SCENE_BUILD_THINGS:
      return SCENE_AI_PILLED;
    case SCENE_AI_PILLED:
      return SCENE_BUILDMAXXING;
    case SCENE_BUILDMAXXING:
      return SCENE_SOFTWARE_FACTORY;
    case SCENE_SOFTWARE_FACTORY:
      return SCENE_TOKENMAXXING;
    case SCENE_TOKENMAXXING:
      return SCENE_BUILD_THINGS;
  }

  return SCENE_BUILD_THINGS;
}

void drawCurrentScene() {
  switch (currentScene) {
    case SCENE_BUILD_THINGS:
      drawBuildThingsScene();
      break;
    case SCENE_AI_PILLED:
      drawAiPilledScene();
      break;
    case SCENE_BUILDMAXXING:
      drawBuildmaxScene();
      break;
    case SCENE_SOFTWARE_FACTORY:
      drawFactoryScene();
      break;
    case SCENE_TOKENMAXXING:
      drawTokenmaxScene();
      break;
  }
}

void setup() {
  Serial.begin(LOG_BAUD_RATE);
  Serial.println(F("LCD Builder Hooks starting"));

  lcd.begin(LCD_COLS, LCD_ROWS);
  showBootAnimation();
  startScene(SCENE_BUILD_THINGS);
}

void loop() {
  unsigned long now = millis();

  if (now - sceneStartedAtMs >= SCENE_DURATION_MS) {
    startScene(nextScene());
  }

  if (now - lastFrameAtMs >= FRAME_INTERVAL_MS) {
    lastFrameAtMs = now;
    drawCurrentScene();
    frame++;
  }
}
