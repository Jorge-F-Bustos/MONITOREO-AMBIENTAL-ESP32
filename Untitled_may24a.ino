#include "arduino_secrets.h"
#include <DHTesp.h>
#include "thingProperties.h"

DHTesp dht;

// Pines LED indicadores de humedad
const int ledVerde = 18;
const int ledRojo = 19;
const int ledAzul = 21;

// Pin LED controlado desde IoT Cloud
const int ledControl = 5;

void setup() {
  Serial.begin(9600);
  delay(1500);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  dht.setup(4, DHTesp::DHT22); // DHT conectado al pin 4

  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  pinMode(ledControl, OUTPUT);
}

void loop() {
  ArduinoCloud.update();

  float temp = dht.getTemperature();
  float hum = dht.getHumidity();

  // Si el LED de control está encendido, reiniciar variables
  if (esp32_relay) {
    dht22_temperatura = 0;
    dht22_humedad = 0;
    digitalWrite(ledControl, HIGH);
  } else {
    dht22_temperatura = temp;
    dht22_humedad = hum;
    digitalWrite(ledControl, LOW);
  }

  // Indicador por LEDs de humedad
  if (hum < 40) {
    digitalWrite(ledRojo, HIGH);
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAzul, LOW);
  } else if (hum <= 60) {
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAzul, LOW);
  } else {
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAzul, HIGH);
  }

  delay(1000);
}

void onEsp32RelayChange() {
}
