#include "Game.h"

namespace Game {

#define FPS 15
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// O display SSD1306 não tem reset pin no teu esquema
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Instalação do objeto do display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Variáveis de controlo de tempo
const unsigned long FrameDelay = 1000 / FPS;
unsigned long lastFrameTime = 0;

// Configuração dos Pinos dos Botões
const int button1Pin = 25;
const int button2Pin = 32;
const int button3Pin = 33;

void init() {
  // Pequena espera para estabilização do hardware
  delay(500);

  // Configurar pinos dos botões com resistência interna Pull-up
  // Ligação física: Pino -> Botão -> GND
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);

  // Inicializar o ecrã OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha ao alocar SSD1306"));
    for (;;);  // Para o código se o ecrã não for detetado
  }

  display.clearDisplay();
  display.display();
}

void showFPS(unsigned long currentDelay) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("FPS: "));

  const unsigned long normalizedDelay = constrain((currentDelay - FrameDelay), 0, 1000);
  const int currentFPS = round((1000 - normalizedDelay) * ((float)FPS / 1000));

  display.println(currentFPS);
}

void loop() {
  const unsigned long now = millis();
  const unsigned long currentDelay = now - lastFrameTime;

  // Garante que o jogo corre à velocidade definida (FPS)
  if (currentDelay < FrameDelay) return;

  lastFrameTime = now;

  // Leitura dos botões (LOW = Pressionado)
  bool btn1 = (digitalRead(button1Pin) == LOW);
  bool btn2 = (digitalRead(button2Pin) == LOW);
  bool btn3 = (digitalRead(button3Pin) == LOW);

  // Aqui podes inserir a lógica de interação:
  // Exemplo: if(btn1) { Pet::fazerAlgo(); }

  display.clearDisplay();

  // Desenha os elementos na tela
  showFPS(currentDelay);

  // Chama a renderização do Pet
  Pet::render();

  display.display();
}

}  // namespace Game