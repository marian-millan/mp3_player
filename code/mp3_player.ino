#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>

// ========== OLED (U8g2) ==========
U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

// ========== MP3 Player ==========
#define MP3_TX 10
#define MP3_RX 11
SoftwareSerial softwareSerial(MP3_TX, MP3_RX);
DFRobotDFPlayerMini player;

// ========== Buttons ==========
#define BUTTON_BACK 2
#define BUTTON_PLAY 3
#define BUTTON_NEXT 4
bool prevBack = HIGH;
bool prevPlay = HIGH;
bool prevNext = HIGH;

// ========== Encoder ==========
#define ENCODER_PIN_A 8
#define ENCODER_PIN_B 9
int prevEncoderPinA;
int currEncoderPinA;

bool isPlaying = true;
bool redraw = true;

// ================= SETUP =================
void setup() {
  Wire.begin();
  Serial.begin(115200);

  buttons_init();
  encoder_init();
  oled_display_init();
  df_player_init();
}

// ================= LOOP =================
void loop() {
  handle_volume();
  handle_buttons();

  bool backHeld = digitalRead(BUTTON_BACK) == LOW;
  bool playHeld = digitalRead(BUTTON_PLAY) == LOW;
  bool nextHeld = digitalRead(BUTTON_NEXT) == LOW;

  // redraw only if something changed
  if (redraw) {
    mp3_player_ui(backHeld, playHeld, nextHeld);
    redraw = false;
  }
}

// ================= INIT =================
void buttons_init() {
  pinMode(BUTTON_BACK, INPUT_PULLUP);
  pinMode(BUTTON_PLAY, INPUT_PULLUP);
  pinMode(BUTTON_NEXT, INPUT_PULLUP);
}

void encoder_init() {
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);
  prevEncoderPinA = digitalRead(ENCODER_PIN_A);
}

void df_player_init() {
  softwareSerial.begin(9600);

  if (player.begin(softwareSerial)) {
    delay(500);
    player.volume(20);
    player.play(1);
  }
}

void oled_display_init() {
  display.begin();

  const char* title = "Starting Up...";

  display.firstPage();
  do {
    display.drawFrame(0, 0, 128, 64);

    display.setFont(u8g2_font_6x10_mf);
    int w = display.getStrWidth(title);
    display.drawStr((128 - w) / 2, 34, title);

    // // Loading bar
    // int barX = 14;
    // int barY = 40;
    // int barWidth = 100;
    // int barHeight = 10;

    // display.drawFrame(barX, barY, barWidth, barHeight);

    // for (int i = 0; i <= barWidth - 2; i++) {
    //   display.drawBox(barX + 1, barY + 1, i, barHeight - 2);
    //   display.sendBuffer();  // force update for animation
    //   delay(1);
    // }
  } while (display.nextPage());
}

// ================= UI =================
void mp3_player_ui(bool backHeld, bool playHeld, bool nextHeld) {
  display.firstPage();
  do {
    display.drawFrame(0, 0, 128, 64);

    // ===== TRACK NAME =====
    char track[20];
    snprintf(track, sizeof(track), "track%03d.mp3", player.readCurrentFileNumber());
    display.setFont(u8g2_font_6x10_mf);
    int w = display.getStrWidth(track);
    display.drawStr((128 - w) / 2, 24, track);

    // ===== BUTTON ROW =====
    int centerY = 44;
    int centerX = 64;
    int spacing = 36;

    draw_back_ui(centerX - spacing, centerY, backHeld);
    draw_play_ui(centerX, centerY, playHeld);
    draw_next_ui(centerX + spacing, centerY, nextHeld);

  } while (display.nextPage());
}

// ================= ICONS =================
void draw_back_ui(int x, int y, bool held) {
  int size = 8;

  if (held) {
    display.drawTriangle(x, y, x + size * 1.5, y - size, x + size * 1.5, y + size);
    display.drawTriangle(x - size * 1.5, y, x, y - size, x, y + size);
  } else {
    display.drawTriangle(x, y, x + size * 1.5, y - size, x + size * 1.5, y + size);
    display.drawTriangle(x - size * 1.5, y, x, y - size, x, y + size);
  }
}

void draw_play_ui(int x, int y, bool held) {
  int size = 8;

  if (isPlaying) {
    if (held) {
      display.drawFrame(x - 5, y - size, 3, size * 2);
      display.drawFrame(x + 2, y - size, 3, size * 2);
    } else {
      display.drawBox(x - 5, y - size, 3, size * 2);
      display.drawBox(x + 2, y - size, 3, size * 2);
    }
  } else {
    if (held) {
      display.drawTriangle(x - size, y - size, x - size, y + size, x + size + 1, y);
    } else {
      display.drawTriangle(x - size, y - size, x - size, y + size, x + size + 1, y);
    }
  }
}

void draw_next_ui(int x, int y, bool held) {
  int size = 8;

  if (held) {
    display.drawTriangle(x, y, x - size * 1.5, y - size, x - size * 1.5, y + size);
    display.drawTriangle(x + size * 1.5, y, x, y - size, x, y + size);
  } else {
    display.drawTriangle(x, y, x - size * 1.5, y - size, x - size * 1.5, y + size);
    display.drawTriangle(x + size * 1.5, y, x, y - size, x, y + size);
  }
}

// ================= INPUT =================
void handle_buttons() {
  bool backState = digitalRead(BUTTON_BACK);
  bool playState = digitalRead(BUTTON_PLAY);
  bool nextState = digitalRead(BUTTON_NEXT);

  // detect ANY change (press or release)
  if (backState != prevBack || playState != prevPlay || nextState != prevNext) {
    redraw = true;
  }

  if (prevBack == LOW && backState == HIGH) {
    player.previous();
    redraw = true;
  }

  if (prevPlay == LOW && playState == HIGH) {
    isPlaying = !isPlaying;
    if (isPlaying) player.start();
    else player.pause();
    redraw = true;
  }

  if (prevNext == LOW && nextState == HIGH) {
    player.next();
    redraw = true;
  }

  prevBack = backState;
  prevPlay = playState;
  prevNext = nextState;
}

void handle_volume() {
  currEncoderPinA = digitalRead(ENCODER_PIN_A);
  if (currEncoderPinA != prevEncoderPinA) {
    if (digitalRead(ENCODER_PIN_B) != currEncoderPinA) {
      player.volumeUp();
    } else {
      player.volumeDown();
    }
  }
  prevEncoderPinA = currEncoderPinA;
}