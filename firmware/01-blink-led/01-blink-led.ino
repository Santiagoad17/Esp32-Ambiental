// Paso 1: LED parpadeando (prueba que la placa se programa bien)
// Usa el LED azul integrado en GPIO 2, no necesita conexiones.

#define LED 2

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(LED, HIGH);
  Serial.println("LED encendido");
  delay(500);
  digitalWrite(LED, LOW);
  Serial.println("LED apagado");
  delay(500);
}
