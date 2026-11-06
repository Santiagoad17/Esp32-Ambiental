# esp32-temperatura

Monitor agrícola con ESP32: mide la **humedad del suelo**, la **temperatura** y la **humedad del aire**, muestra los datos en una pantalla OLED y enciende un LED de alarma cuando hay que regar o hace demasiado calor.

```
┌──────────────────────┐
│  Aire:     24.5 C    │
│  Hum aire: 68 %      │
│  Suelo:    45 %      │
│  OK                  │
└──────────────────────┘
```

## Componentes

| Componente | Uso |
|---|---|
| ESP32 DevKit V1 (30 pines, WROOM-32) | Microcontrolador |
| SHT31 (I2C) | Temperatura y humedad del aire |
| Sensor de humedad de suelo capacitivo v1.2 | Humedad del suelo |
| Pantalla OLED 0.96" I2C (SSD1306) | Mostrar los datos |
| LED + resistencia de 220–330 Ω | Alarma (opcional: la placa ya trae un LED en GPIO 2) |
| 2 protoboards de 830 puntos y cables Dupont | Montaje |

Ver [docs/lista-compras.md](docs/lista-compras.md).

## Conexiones

| Componente | Pin | ESP32 |
|---|---|---|
| SHT31 | VIN / GND | 3.3V / GND |
| | SDA / SCL | GPIO 21 / GPIO 22 |
| OLED | VCC / GND | 3.3V / GND |
| | SDA / SCL | GPIO 21 / GPIO 22 (compartidos con el SHT31) |
| Sensor de suelo | VCC / GND | 3.3V / GND |
| | AOUT | GPIO 34 |
| LED | Pata larga → resistencia de 220 Ω | GPIO 2 |
| | Pata corta | GND |

> ⚠️ Los pines del ESP32 trabajan a 3.3 V. Usa solo GPIO 32–39 para las lecturas analógicas, porque los demás pines analógicos no funcionan cuando el WiFi está activo.

## Firmware

| Sketch | Qué hace |
|---|---|
| [firmware/01-blink-led](firmware/01-blink-led) | Hace parpadear el LED para probar que la placa se programa bien |
| [firmware/02-calibracion-suelo](firmware/02-calibracion-suelo) | Calibra el sensor de suelo con la prueba del vaso |
| [firmware/esp32-temperatura](firmware/esp32-temperatura) | Monitor completo con los sensores, la pantalla y la alarma |

### Preparar Arduino IDE

1. Instala [Arduino IDE](https://www.arduino.cc/en/software).
2. En *Archivo → Preferencias → URLs adicionales de gestor de placas* agrega:
   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
3. En *Herramientas → Placa → Gestor de placas* instala **esp32** (Espressif).
4. En *Herramientas → Administrar bibliotecas* instala:
   - Adafruit SHT31 Library
   - Adafruit SSD1306
   - Adafruit GFX Library
5. Selecciona la placa **ESP32 Dev Module** y el puerto COM correcto.
6. Si la carga se queda en `Connecting...`, mantén presionado el botón **BOOT**.

Si Windows no detecta la placa, instala el driver **CP210x** o **CH340**, según el chip USB de tu placa, y verifica que el cable USB sea de datos.

## Calibración del sensor de suelo (prueba del vaso)

1. Sube `02-calibracion-suelo` y abre el Monitor Serie (115200).
2. Con el sensor **al aire**, anota la lectura. Ese es tu `valorSeco`.
3. Mete el sensor **en un vaso de agua, solo hasta la línea blanca**, y anota la lectura. Ese es tu `valorMojado`.
4. Escribe esos valores en `esp32-temperatura.ino`.

> Más agua da un número más bajo. Nunca mojes la parte de los componentes electrónicos.

## Orden de prueba

1. Solo el ESP32: el LED parpadea.
2. Sensor de suelo: prueba del vaso.
3. SHT31: la temperatura sale en el Monitor Serie (si soplas el sensor, la humedad sube).
4. OLED: los datos aparecen en la pantalla.
5. Todo junto: si sacas el sensor de suelo al aire, el LED se enciende y la pantalla dice "REGAR!".

## Próximos pasos

- [ ] Relé y bomba para riego automático
- [ ] Alertas por WiFi (Telegram o WhatsApp)
- [ ] Gráficas del historial en la web
- [ ] Caja IP65 y batería con panel solar para campo
- [ ] Migrar a ESP32-S3 o LoRa (Heltec WiFi LoRa 32 V3)
