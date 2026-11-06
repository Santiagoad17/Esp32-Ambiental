// =============================================================
//  Monitor agricola: suelo y clima con ESP32
//  - Humedad del suelo (sensor capacitivo v1.2)
//  - Temperatura y humedad del aire (SHT31)
//  - Pantalla OLED 0.96" I2C (SSD1306)
//  - LED de alarma
// =============================================================

#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ----- Pines -----
#define PIN_SUELO 34   // usar solo GPIO 32-39 para analogicos (ADC1)
#define PIN_LED   2    // LED integrado de la placa

// ----- Calibracion del sensor de suelo (prueba del vaso) -----
// Usa el sketch 02-calibracion-suelo para obtener tus valores
int valorSeco   = 3000;   // sensor al aire
int valorMojado = 1300;   // sensor en el vaso de agua (hasta la linea)

// ----- Limites de alarma -----
int   sueloMinimo = 30;     // % por debajo = hay que regar
float tempMaxima  = 30.0;   // C por encima = mucho calor

Adafruit_SHT31 sht31;
Adafruit_SSD1306 pantalla(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  Wire.begin(21, 22);   // SDA, SCL (compartidos por SHT31 y OLED)

  if (!sht31.begin(0x44)) {
    Serial.println("No se encontro el SHT31. Revisa conexiones.");
  }
  if (!pantalla.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("No se encontro la pantalla OLED. Revisa conexiones.");
  }

  pantalla.clearDisplay();
  pantalla.setTextColor(SSD1306_WHITE);
  pantalla.setTextSize(1);
  pantalla.setCursor(10, 25);
  pantalla.println("Monitor Agricola");
  pantalla.display();
  delay(2000);
}

void loop() {
  // --- Leer sensores ---
  float temp    = sht31.readTemperature();
  float humAire = sht31.readHumidity();

  int lecturaSuelo = analogRead(PIN_SUELO);
  int humSuelo = map(lecturaSuelo, valorSeco, valorMojado, 0, 100);
  humSuelo = constrain(humSuelo, 0, 100);

  // --- Revisar alarmas ---
  String estado = "OK";
  bool alarma = false;

  if (humSuelo < sueloMinimo) {
    estado = "REGAR!";
    alarma = true;
  }
  if (temp > tempMaxima) {
    estado = "MUCHO CALOR!";
    alarma = true;
  }

  digitalWrite(PIN_LED, alarma ? HIGH : LOW);

  // --- Monitor Serie ---
  Serial.printf("Temp: %.1f C | Hum aire: %.1f %% | Suelo: %d %% (lectura %d) | %s\n",
                temp, humAire, humSuelo, lecturaSuelo, estado.c_str());

  // --- Pantalla ---
  pantalla.clearDisplay();
  pantalla.setTextSize(1);

  pantalla.setCursor(0, 0);
  pantalla.printf("Aire:     %.1f C", temp);

  pantalla.setCursor(0, 14);
  pantalla.printf("Hum aire: %.0f %%", humAire);

  pantalla.setCursor(0, 28);
  pantalla.printf("Suelo:    %d %%", humSuelo);

  pantalla.setCursor(0, 46);
  pantalla.setTextSize(2);
  pantalla.print(estado);

  pantalla.display();

  delay(2000);
}
