#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(D2);
DallasTemperature sensors(&oneWire);

const char* ssid = "BARAMUN-7IRUH9L 6588";
const char* password = "12345678";

const char* serverName = "http://172.22.4.33/esp8266_project/test_data.php";
WiFiClient client;

float getTemperature() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0)/-10;
  return temperatureC;
}

void setup() {
  sensors.begin();
  HTTPClient http;
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  if (WiFi.status() == WL_CONNECTED) {
    while (true) {
      float temperature = getTemperature();

      String url = serverName;
      url += "?temperature=";
      url += String(temperature);

      http.begin(client, url);

      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(response);
      } else {
        Serial.print("Error on sending GET: ");
        Serial.println(httpResponseCode);
      }

      http.end();
      
      delay(30000); 
    }
  } 
}

void loop() {
   // wait for 30 seconds before sending the nextr equest
}