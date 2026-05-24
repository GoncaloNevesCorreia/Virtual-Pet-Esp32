#include "Game.h"

namespace Game {

#define FPS 15
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// O display SSD1306 não tem reset pin.
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned long FrameDelay = 1000 / FPS;
unsigned long lastFrameTime = 0;

// Configuração dos Pinos dos Botões
Button btnFeed(4);
Button btnPlay(5);
Button btnSleep(6);

Timer autoSaveTimer(save, 10000);

void init() {
  // Wait for display
  delay(500);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha ao alocar SSD1306"));
    for (;;);  // Don't proceed, loop forever
  }

  Pet::init();

  btnFeed.onClick(Pet::eat);
  btnPlay.onClick(Pet::play);
  btnSleep.onClick(Pet::toggleSleep);
}

void showFPS(unsigned long currentDelay) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(48, 4);
  display.print(F("FPS:"));

  if (currentDelay > 0) {
    const int currentFPS = round(1000 / (float)currentDelay);
    display.println(currentFPS);
  } else {
    display.println(0);
  }
}

void draw() {
  Pet::render();
}

void loop() {
  const unsigned long now = millis();
  const unsigned long currentDelay = now - lastFrameTime;

  btnFeed.read();
  btnPlay.read();
  btnSleep.read();

  if (currentDelay < FrameDelay) return;

  lastFrameTime = now;

  display.clearDisplay();

  showFPS(currentDelay);

  draw();

  display.display();

  autoSaveTimer.run();
}

void save() {
  Pet_Energy::save();
  Pet_Fun::save();
  Pet_Health::save();
  Pet_Hunger::save();
}

}  // namespace Game