#include "Engine.h"

namespace Engine {

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// O display SSD1306 não tem reset pin.
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum PetState currentPetState = PetState::DEAD;

int currentFrame = 0;
unsigned long lastAnimationTime = 0;
int animationDelay = 500;  // Switch frame every 500ms

void init() {
  // Wait for display
  delay(500);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }
}

void SetPetState(enum PetState state) {
  currentPetState = state;
}

void RenderPet() {
  if (millis() - lastAnimationTime > animationDelay) {
    lastAnimationTime = millis();

    display.clearDisplay();  // Clear screen

    display.drawBitmap(48, 16, bitmap_pet_dead[currentFrame], 32, 32, 1);

    display.display();  // Push to screen

    // Toggle between frames
    currentFrame = (currentFrame + 1) % 3;
  }
}

}  // namespace Engine