#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ACS758
const int sensorPin = A0;

// 80Ah akku
const float batteryCapacityAh = 80.0;

// ACS758-050B
const float sensitivity = 0.040;

// Kalibroituu automaattisesti
float zeroCurrentVoltage = 2.50;

float current = 0.0;
float usedAh = 0.0;

unsigned long lastTime = 0;

void setup() {

  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.display();

  // =========================
  // AUTOMAATTINEN KALIBROINTI
  // =========================

  long sum = 0;

  for (int i = 0; i < 500; i++) {

    sum += analogRead(sensorPin);
    delay(2);
  }

  float avg = sum / 500.0;

  zeroCurrentVoltage = avg * (5.0 / 1023.0);

  Serial.print("Zero voltage: ");
  Serial.println(zeroCurrentVoltage, 3);

  lastTime = millis();
}

void loop() {

  // =========================
  // MITTAA VIRTA
  // =========================

  long sum = 0;

  for (int i = 0; i < 50; i++) {

    sum += analogRead(sensorPin);
  }

  float raw = sum / 50.0;

  float voltage = raw * (5.0 / 1023.0);

  current = (voltage - zeroCurrentVoltage) / sensitivity;

  // Poista pieni kohina
  if (abs(current) < 0.10) {
    current = 0.0;
  }

  // =========================
  // LASKE Ah
  // =========================

  unsigned long now = millis();

  float hoursPassed = (now - lastTime) / 3600000.0;

  lastTime = now;

  if (current > 0) {
    usedAh += current * hoursPassed;
  }

  // =========================
  // OLED
  // =========================

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  // OTSIKKO
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Kulutettu");

  // ISO LUKEMA
  // Viela mahtuu keltaiselle alueelle
  display.setTextSize(2);

  display.setCursor(0, 12);

  // 3 desimaalia
  display.print(usedAh, 3);

  // YKSIKKO
  display.print("Ah");

  // VIRTA
  display.setTextSize(1);

  display.setCursor(0, 44);
  display.print("Virta: ");

  display.print(current, 2);

  display.print(" A");

  display.display();

  // =========================
  // DEBUG
  // =========================

  Serial.print("Voltage: ");
  Serial.print(voltage, 3);

  Serial.print("  Current: ");
  Serial.print(current, 2);

  Serial.print(" A");

  Serial.print("  Used: ");
  Serial.print(usedAh, 4);

  Serial.println(" Ah");

  delay(200);
}
