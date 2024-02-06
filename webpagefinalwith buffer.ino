#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char* ssid = "redminote9s";
const char* password = "PraktikuM!";

ESP8266WebServer server(80);
OneWire oneWire(D2);
DallasTemperature sensors(&oneWire);

const int MAX_BUFFER_SIZE = 50;
int bufferIndex = 0;
String buffer[MAX_BUFFER_SIZE];

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    Serial.print(".");
  }

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    String time = getTime();
    float temperatureC = getTemperature();
    String html = time + "</p><p>Current temperature: " + String(temperatureC) + " C</p>";
    server.send(200, "text/html", html);
  });

  server.on("/api/hello", []() {
    String time = getTime();
    float temperatureC = getTemperature();
    String json = "{"message":"Hello World","time":"" + time + "","temperature":"" + String(temperatureC) + ""}";
    server.send(200, "application/json", json);
  });

  server.on("/api/helloworld", []() {
    String time = getTime();
    float temperatureC = getTemperature();
    String content = "Hello World\nCurrent time: " + time + "\nCurrent temperature: " + String(temperatureC) + " C";
    server.sendHeader("Content-Disposition", "attachment; filename=helloworld.txt");
    server.send(200, "text/plain", content);
  });

  server.begin();
}

void loop() {
  server.handleClient();

  if (bufferIndex >= MAX_BUFFER_SIZE) {
    bufferIndex = 0;
  }

  String time = getTime();
  float temperatureC = getTemperature();
  String data = time + " " + String(temperatureC) + " C\n";
  buffer[bufferIndex] = data;
  bufferIndex++;

  writeBufferToFile();
}

String getTime() {
  configTime(0, 0, "pool.ntp.org");
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char time[20];
  strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", &timeinfo);

  return String(time);
}

float getTemperature() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0)/-10;
  return temperatureC;
}

void writeBufferToFile() {
  File file = SPIFFS.open("/helloworld.txt", "w");

  for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
    if (buffer[i] != "") {
      file.println(buffer[i]);
    }
  }

  file.close();
}