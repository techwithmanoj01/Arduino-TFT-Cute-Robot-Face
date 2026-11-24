/*
  CUTE TFT ROBOT FACE - VERSION 2
  Arduino Uno + 2.4-inch MCUFRIEND TFT Shield

  20 automatic cute face animations
  Resolution: 320 x 240 (Landscape)

  Libraries:
  MCUFRIEND_kbv
  Adafruit GFX Library

  No SD card or extra components needed.
*/

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

// Colours
const uint16_t BG     = 0xDFFF;
const uint16_t INK    = 0x10A4;
const uint16_t PINK   = 0xF4BA;
const uint16_t RED    = 0xF920;
const uint16_t BLUE   = 0x049F;
const uint16_t YELLOW = 0xFFE0;

// Face positions
const int16_t LX = 93;
const int16_t RX = 227;
const int16_t EY = 97;
const int16_t CX = 160;

enum EyeType {
  E_OPEN,
  E_HALF,
  E_CLOSED,
  E_HEART,
  E_STAR,
  E_ANGRY,
  E_SAD,
  E_HIDDEN
};

enum MouthType {
  M_SMILE,
  M_GRIN,
  M_O,
  M_FROWN,
  M_FLAT,
  M_TONGUE,
  M_SLEEP,
  M_CAT
};

enum ExtraType {
  X_NONE,
  X_HEART,
  X_HEARTS,
  X_TEAR,
  X_CRY,
  X_ANGER,
  X_ZZZ,
  X_SPARKLE
};

// Function prototypes
void curve(int16_t, int16_t, int16_t,
           int16_t, int16_t, int16_t, uint8_t);

void heart(int16_t, int16_t, int8_t, uint16_t);
void star(int16_t, int16_t, int16_t, uint16_t);
void eyelash(int16_t);
void eye(int16_t, EyeType, int8_t, int8_t, bool);
void mouth(MouthType);
void tear(int16_t, int16_t);

void face(EyeType, EyeType, MouthType,
          int8_t, int8_t, bool, ExtraType, uint8_t);

void show(EyeType, EyeType, MouthType,
          uint16_t, int8_t, int8_t,
          bool, ExtraType, uint8_t);

void playScene(uint8_t);

// Draw smooth curves
void curve(int16_t x0, int16_t y0,
           int16_t x1, int16_t y1,
           int16_t x2, int16_t y2,
           uint8_t thick = 2) {

  int16_t px = x0;
  int16_t py = y0;

  for (int8_t i = 1; i <= 16; i++) {

    int32_t a = 16 - i;
    int32_t b = i;

    int16_t x =
      (a*a*x0 + 2*a*b*x1 + b*b*x2) / 256;

    int16_t y =
      (a*a*y0 + 2*a*b*y1 + b*b*y2) / 256;

    for (uint8_t j = 0; j < thick; j++) {
      tft.drawLine(px, py+j, x, y+j, INK);
    }

    px = x;
    py = y;
  }
}

// Draw hearts
void heart(int16_t x, int16_t y,
           int8_t r, uint16_t color) {

  tft.fillCircle(x-r, y, r, color);
  tft.fillCircle(x+r, y, r, color);

  tft.fillTriangle(
    x-2*r, y,
    x+2*r, y,
    x, y+2*r+3,
    color
  );
}

// Draw stars
void star(int16_t x, int16_t y,
          int16_t r, uint16_t color) {

  int16_t vx[10], vy[10];

  vx[0]=0;     vy[0]=-r;
  vx[1]=r/4;   vy[1]=-r/4;
  vx[2]=r;     vy[2]=-r/4;
  vx[3]=r/3;   vy[3]=r/5;
  vx[4]=r/2;   vy[4]=r;
  vx[5]=0;     vy[5]=r/2;
  vx[6]=-r/2;  vy[6]=r;
  vx[7]=-r/3;  vy[7]=r/5;
  vx[8]=-r;    vy[8]=-r/4;
  vx[9]=-r/4;  vy[9]=-r/4;

  for (uint8_t i = 0; i < 10; i++) {

    uint8_t j = (i + 1) % 10;

    tft.fillTriangle(
      x, y,
      x+vx[i], y+vy[i],
      x+vx[j], y+vy[j],
      color
    );
  }
}

// Cute eyelashes
void eyelash(int16_t x) {

  curve(
    x-27, EY+38,
    x, EY+66,
    x+27, EY+38,
    2
  );

  tft.drawLine(
    x-27, EY+38,
    x-34, EY+34,
    INK
  );
}

// Draw different eye expressions
void eye(int16_t x, EyeType kind,
         int8_t dx, int8_t dy,
         bool isLeft) {

  int16_t ex = x + dx;
  int16_t ey = EY + dy;

  switch (kind) {

    case E_OPEN:
      tft.fillCircle(ex, ey, 31, INK);
      tft.fillCircle(ex-11, ey-11, 11, BG);
      tft.fillCircle(ex+9, ey+9, 4, BG);
      eyelash(x);
      break;

    case E_HALF:
      tft.fillRoundRect(
        x-30, EY+1, 60, 16, 8, INK
      );

      tft.fillCircle(x-10, EY+5, 5, BG);
      eyelash(x);
      break;

    case E_CLOSED:
      curve(
        x-28, EY+7,
        x, EY+30,
        x+28, EY+7,
        3
      );

      tft.drawLine(
        x-28, EY+7,
        x-34, EY+3,
        INK
      );
      break;

    case E_HEART:
      heart(x, EY-7, 16, RED);
      tft.fillCircle(x-12, EY-12, 5, BG);
      break;

    case E_STAR:
      star(x, EY, 32, YELLOW);
      tft.fillCircle(x-7, EY-8, 4, BG);
      break;

    case E_ANGRY:
      tft.fillCircle(x, EY+8, 25, INK);
      tft.fillCircle(x-8, EY+1, 8, BG);

      if (isLeft) {
        tft.drawLine(x-29, EY-29,
                     x+26, EY-14, INK);
        tft.drawLine(x-29, EY-30,
                     x+26, EY-15, INK);
        tft.drawLine(x-29, EY-31,
                     x+26, EY-16, INK);
      } else {
        tft.drawLine(x-26, EY-14,
                     x+29, EY-29, INK);
        tft.drawLine(x-26, EY-15,
                     x+29, EY-30, INK);
        tft.drawLine(x-26, EY-16,
                     x+29, EY-31, INK);
      }
      break;

    case E_SAD:
      tft.fillCircle(x, EY+9, 24, INK);
      tft.fillCircle(x-9, EY+1, 8, BG);

      if (isLeft) {
        tft.drawLine(x-28, EY-25,
                     x+24, EY-35, INK);
        tft.drawLine(x-28, EY-26,
                     x+24, EY-36, INK);
      } else {
        tft.drawLine(x-24, EY-35,
                     x+28, EY-25, INK);
        tft.drawLine(x-24, EY-36,
                     x+28, EY-26, INK);
      }
      break;

    case E_HIDDEN:
      // Eye disappears
      break;
  }
}

// Different mouth expressions
void mouth(MouthType kind) {

  switch (kind) {

    case M_SMILE:
      curve(CX-11, 163,
            CX, 185,
            CX+11, 163, 3);
      break;

    case M_GRIN:
      curve(CX-19, 160,
            CX, 199,
            CX+19, 160, 3);
      break;

    case M_O:
      tft.fillCircle(CX, 175, 12, INK);
      tft.fillCircle(CX-4, 171, 3, BG);
      break;

    case M_FROWN:
      curve(CX-16, 188,
            CX, 161,
            CX+16, 188, 3);
      break;

    case M_FLAT:
      tft.drawFastHLine(
        CX-15, 176, 30, INK
      );
      tft.drawFastHLine(
        CX-15, 177, 30, INK
      );
      break;

    case M_TONGUE:
      tft.fillRoundRect(
        CX-15, 161, 30, 30, 11, INK
      );
      tft.fillCircle(CX, 181, 9, PINK);
      break;

    case M_SLEEP:
      tft.fillCircle(CX, 173, 5, INK);
      break;

    case M_CAT:
      curve(CX-19, 165,
            CX-9, 184,
            CX, 165, 2);

      curve(CX, 165,
            CX+9, 184,
            CX+19, 165, 2);
      break;
  }
}

// Draw falling tears
void tear(int16_t x, int16_t y) {

  tft.fillTriangle(
    x, y-10,
    x-6, y+2,
    x+6, y+2,
    BLUE
  );

  tft.fillCircle(x, y+2, 6, BLUE);
  tft.fillCircle(x-2, y+1, 2, BG);
}

// Draw complete robot face
void face(EyeType left,
          EyeType right,
          MouthType m,
          int8_t dx = 0,
          int8_t dy = 0,
          bool blush = false,
          ExtraType extra = X_NONE,
          uint8_t phase = 0) {

  // Clear face regions
  tft.fillRect(111, 1, 98, 46, BG);
  tft.fillRect(126, 146, 68, 66, BG);
  tft.fillRect(41, 45, 104, 145, BG);
  tft.fillRect(175, 45, 104, 145, BG);

  eye(LX, left, dx, dy, true);
  eye(RX, right, dx, dy, false);

  // Blushing cheeks
  if (blush) {
    tft.fillCircle(132, 151, 9, PINK);
    tft.fillCircle(188, 151, 9, PINK);
  }

  mouth(m);

  // Extra animations
  switch (extra) {

    case X_HEART:
      heart(160, 20, 7, RED);
      break;

    case X_HEARTS:
      heart(145, 33-phase*3, 6, RED);
      heart(178, 39-phase*2, 5, PINK);
      break;

    case X_TEAR:
      tear(LX+28, EY+53+phase*3);
      break;

    case X_CRY:
      tear(LX+28, EY+53+phase*3);
      tear(RX-28, EY+53+phase*3);
      break;

    case X_ANGER:
      tft.drawLine(149, 16, 158, 26, RED);
      tft.drawLine(158, 26, 171, 16, RED);
      tft.drawLine(147, 29, 160, 25, RED);
      tft.drawLine(160, 25, 174, 29, RED);
      break;

    case X_ZZZ:
      tft.setTextSize(2);
      tft.setTextColor(INK);
      tft.setCursor(137+(phase%2)*2, 10);
      tft.print(F("Zzz"));
      break;

    case X_SPARKLE:
      star(143, 22, 10, YELLOW);
      star(180, 29, 7, YELLOW);
      break;

    case X_NONE:
      break;
  }
}

// Show expression for a duration
void show(EyeType l,
          EyeType r,
          MouthType m,
          uint16_t ms,
          int8_t dx = 0,
          int8_t dy = 0,
          bool blush = false,
          ExtraType ex = X_NONE,
          uint8_t phase = 0) {

  face(l, r, m, dx, dy, blush, ex, phase);
  delay(ms);
}

// 20 Animation Scenes
void playScene(uint8_t scene) {

  int8_t i;

  switch (scene) {

    // 01. Wake up
    case 0:
      show(E_CLOSED, E_CLOSED, M_SLEEP, 400);
      show(E_HALF, E_HALF, M_SMILE, 250);
      show(E_OPEN, E_OPEN, M_SMILE, 850);
      break;

    // 02. Double blinking
    case 1:
      for (uint8_t b = 0; b < 2; b++) {
        show(E_HALF, E_HALF, M_SMILE, 90);
        show(E_CLOSED, E_CLOSED, M_SMILE, 140);
        show(E_OPEN, E_OPEN, M_SMILE, 200);
      }
      break;

    // 03. Left wink
    case 2:
      show(E_OPEN, E_OPEN, M_CAT, 220);
      show(E_CLOSED, E_OPEN, M_CAT,
           630, 0, 0, true);
      show(E_OPEN, E_OPEN, M_SMILE, 350);
      break;

    // 04. Right wink
    case 3:
      show(E_OPEN, E_OPEN, M_CAT, 220);
      show(E_OPEN, E_CLOSED, M_CAT,
           630, 0, 0, true);
      show(E_OPEN, E_OPEN, M_SMILE, 350);
      break;

    // 05. Look left
    case 4:
      for (i = 0; i >= -12; i -= 3) {
        show(E_OPEN, E_OPEN, M_SMILE,
             110, i, 0);
      }

      delay(400);

      for (i = -9; i <= 0; i += 3) {
        show(E_OPEN, E_OPEN, M_SMILE,
             90, i, 0);
      }
      break;

    // 06. Look right
    case 5:
      for (i = 0; i <= 12; i += 3) {
        show(E_OPEN, E_OPEN, M_SMILE,
             110, i, 0);
      }

      delay(400);

      for (i = 9; i >= 0; i -= 3) {
        show(E_OPEN, E_OPEN, M_SMILE,
             90, i, 0);
      }
      break;

    // 07. Look up
    case 6:
      for (i = 0; i >= -10; i -= 2) {
        show(E_OPEN, E_OPEN, M_O,
             110, 0, i);
      }

      delay(450);
      show(E_OPEN, E_OPEN, M_SMILE, 350);
      break;

    // 08. Look down
    case 7:
      for (i = 0; i <= 10; i += 2) {
        show(E_OPEN, E_OPEN, M_SMILE,
             110, 0, i);
      }

      delay(450);
      show(E_OPEN, E_OPEN, M_SMILE, 350);
      break;

    // 09. Shy blush
    case 8:
      show(E_OPEN, E_OPEN, M_SMILE, 250);
      show(E_OPEN, E_OPEN, M_CAT,
           350, 0, 0, true);
      show(E_HALF, E_HALF, M_CAT,
           380, 0, 0, true);
      show(E_OPEN, E_OPEN, M_CAT,
           650, 0, 0, true);
      break;

    // 10. Happy laughing
    case 9:
      show(E_OPEN, E_OPEN, M_SMILE,
           260, 0, 0, true);

      show(E_OPEN, E_OPEN, M_GRIN,
           410, 0, 0, true);

      show(E_CLOSED, E_CLOSED, M_TONGUE,
           480, 0, 0, true);

      show(E_OPEN, E_OPEN, M_GRIN,
           650, 0, 0, true);
      break;

    // 11. Heart eyes
    case 10:
      show(E_OPEN, E_OPEN, M_O, 220);

      show(E_HEART, E_HEART, M_GRIN,
           350, 0, 0, true, X_HEART);

      show(E_HEART, E_HEART, M_CAT,
           800, 0, 0, true, X_HEART);
      break;

    // 12. Floating hearts
    case 11:
      for (uint8_t p = 0; p <= 6; p++) {
        show(E_OPEN, E_OPEN, M_CAT,
             210, 0, 0, true, X_HEARTS, p);
      }
      break;

    // 13. Sad face with tear
    case 12:
      show(E_OPEN, E_OPEN, M_FLAT, 200);

      for (uint8_t p = 0; p <= 4; p++) {
        show(E_SAD, E_SAD, M_FROWN,
             230, 0, 0, false, X_TEAR, p);
      }
      break;

    // 14. Crying animation
    case 13:
      for (uint8_t p = 0; p <= 5; p++) {
        show(E_SAD, E_SAD, M_FROWN,
             230, 0, 0, false, X_CRY, p);
      }

      show(E_CLOSED, E_CLOSED,
           M_FROWN, 440);
      break;

    // 15. Angry expression
    case 14:
      show(E_OPEN, E_OPEN, M_FLAT, 230);

      show(E_ANGRY, E_ANGRY, M_FROWN,
           740, 0, 0, false, X_ANGER);

      show(E_ANGRY, E_ANGRY, M_FLAT,
           450, 0, 0, false, X_ANGER);
      break;

    // 16. Surprised face
    case 15:
      show(E_OPEN, E_OPEN, M_O,
           450, 0, -6);

      show(E_OPEN, E_OPEN, M_O,
           320, 0, 4);

      show(E_OPEN, E_OPEN, M_O,
           550, 0, -6);
      break;

    // 17. Feeling sleepy
    case 16:
      show(E_OPEN, E_OPEN, M_SLEEP, 180);
      show(E_HALF, E_HALF, M_SLEEP, 350);
      show(E_CLOSED, E_CLOSED, M_SLEEP, 390);
      show(E_HALF, E_HALF, M_SLEEP, 350);
      break;

    // 18. Sleeping with Zzz
    case 17:
      for (uint8_t p = 0; p < 5; p++) {
        show(E_CLOSED, E_CLOSED, M_SLEEP,
             380, 0, 0, false, X_ZZZ, p);
      }
      break;

    // 19. Sparkling star eyes
    case 18:
      show(E_HALF, E_HALF, M_SMILE, 220);

      show(E_STAR, E_STAR, M_O,
           400, 0, 0, true, X_SPARKLE);

      show(E_STAR, E_STAR, M_GRIN,
           700, 0, 0, true, X_SPARKLE);
      break;

    // 20. Disappearing eyes
    case 19:
      show(E_OPEN, E_OPEN, M_SMILE, 350);

      show(E_OPEN, E_HIDDEN,
           M_SMILE, 480);

      show(E_HIDDEN, E_OPEN,
           M_CAT, 480);

      show(E_HIDDEN, E_HIDDEN,
           M_O, 340);

      show(E_HALF, E_HALF,
           M_SMILE, 200);

      show(E_OPEN, E_OPEN, M_GRIN,
           850, 0, 0, true);
      break;
  }
}

// Arduino setup
void setup() {

  Serial.begin(9600);

  uint16_t id = tft.readID();

  Serial.print(F("Detected TFT controller: 0x"));
  Serial.println(id, HEX);

  // Common ILI9341 fallback
  if (id == 0xD3D3 ||
      id == 0xFFFF ||
      id == 0x0000) {
    id = 0x9341;
  }

  tft.begin(id);

  // Landscape display
  tft.setRotation(1);

  // Light blue background
  tft.fillScreen(BG);

  // Initial cute face
  show(E_OPEN, E_OPEN, M_SMILE, 1200);
}

// Main animation loop
void loop() {

  for (uint8_t scene = 0; scene < 20; scene++) {

    playScene(scene);

    delay(250);
  }

  // Repeat all 20 animations
}
