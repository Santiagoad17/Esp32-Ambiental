// Paso 2: Calibracion del sensor de humedad de suelo capacitivo (prueba del vaso)
//
// 1. Sube este codigo y abre el Monitor Serie (115200).
// 2. Sensor al aire        -> anota la "Lectura" = valorSeco
// 3. Sensor en vaso de agua (solo hasta la linea blanca) -> anota = valorMojado
// 4. Copia esos dos valores aqui y en esp32-temperatura.ino

#define SENSOR_SUELO 34

int valorSeco   = 3000;
int valorMojado = 1300;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int lectura = analogRead(SENSOR_SUELO);

  int humedad = map(lectura, valorSeco, valorMojado, 0, 100);
  humedad = constrain(humedad, 0, 100);

  Serial.printf("Lectura: %d  ->  Humedad suelo: %d %%\n", lectura, humedad);

  if (humedad < 30) {
    Serial.println("Suelo seco! Hay que regar");
  }

  delay(1000);
}
