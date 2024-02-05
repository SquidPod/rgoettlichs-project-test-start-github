#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN D4 // Pin, an dem der DHT22-Sensor angeschlossen ist
#define DHTTYPE DHT22 // Sensortyp

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("DHT22 Temperatur- und Luftfeuchtigkeitsmessung");

  dht.begin();
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Fehler beim Lesen der Sensordaten");
    return;
  }


}