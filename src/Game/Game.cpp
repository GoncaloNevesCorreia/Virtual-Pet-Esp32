#include "Game.h"

namespace Game {

#define FPS 15
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// O display SSD1306 não tem reset pin.
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define AUTO_SAVE_INTERVAL = 10000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned long FrameDelay = 1000 / FPS;
unsigned long lastFrameTime = 0;

// Configuração dos Pinos dos Botões
Button btnFeed(4);
Button btnPlay(5);
Button btnSleep(6);

void onStateChange();

void save();

// TODO: Implement Auto-Save on state change.
Timer autoSaveTimer(save, 10000);

Timer autoSendStats(Network::sendStats, 1000);

void init() {
  // Wait for display
  delay(500);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha ao alocar SSD1306"));
    for (;;);  // Don't proceed, loop forever
  }

  Pet_Energy::init();
  Pet_Fun::init();
  Pet_Health::init();
  Pet_Hunger::init();

  Pet::init();

  Pet::onStateChange(onStateChange);

  btnFeed.onClick(eat);
  btnPlay.onClick(play);
  btnSleep.onClick(sleep);
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
  Pet_Energy::render();
  Pet_Fun::render();
  Pet_Health::render();
  Pet_Hunger::render();

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
  autoSendStats.run();
}

void save() {
  Pet_Energy::save();
  Pet_Fun::save();
  Pet_Health::save();
  Pet_Hunger::save();
}

void restart() {
  Pet_Energy::clear();
  Pet_Fun::clear();
  Pet_Health::clear();
  Pet_Hunger::clear();

  Pet_Energy::init();
  Pet_Fun::init();
  Pet_Health::init();
  Pet_Hunger::init();

  Pet::init();
}

void eat() {
  if (Pet::isDead()) {
    restart();
    return;
  }

  Pet::eat();
}

void play() {
  if (Pet::isDead()) {
    restart();
    return;
  }

  Pet::play();
}

void sleep() {
  if (Pet::isDead()) {
    restart();
    return;
  }

  Pet::toggleSleep();
}

void getStats(uint8_t stats[5]) {
  stats[0] = Pet::getState();
  stats[1] = Pet_Health::health;
  stats[2] = Pet_Hunger::hunger;
  stats[3] = Pet_Energy::energy;
  stats[4] = Pet_Fun::fun;
}

void onStateChange() {
  save();
  autoSaveTimer.reset();
}

}  // namespace Game