#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char* ssid = "redminote9s";
const char* password = "!PraktikuM";

ESP8266WebServer server(80);
OneWire oneWire(D2);
DallasTemperature sensors(&oneWire);

float tempArray[51];
int tempIndex = 0;
void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(BUILTIN_LED, LOW);
    Serial.print(".");
  }
  digitalWrite(BUILTIN_LED, HIGH);
  
  Serial.println("");
  Serial.println("WiFi connected.");
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
    String json = "{\"Hello World\":\"Current time: " + String(time) + "\",\"Current temperature\":\"" + String(temperatureC) + " C\"}";
    server.send(200, "application/json", json);
  });
 
  server.on("/api/helloworld", []() {
      String time = getTime();
      String content = "Current temperature values: ";
      for (int i = 0; i < tempIndex; i++) {
        content += "\n " + String(time) + "  :  " + String(tempArray[i]) + " C" ;
      }
      server.sendHeader("Content-Disposition", "attachment; filename=helloworld.txt");
      server.send(200, "text/plain " , content);
  });
 
  server.on("/api/start", []() {
    String html_1 = "Hello everything link: <a href='http://192.168.56.247/api/hello'><button>Click me</button></a>";
    String html_2 = "<br>download data link: <a href='http://192.168.56.247/api/helloworld'><button>Click me</button></a>";
    server.send(200, "text/html", html_1 + html_2);
  });
 
  // Start the web server/tempsensors
  server.begin();
  sensors.begin();
}

void loop() {
  
  server.handleClient();

  sensors.requestTemperatures();

  float temperatureC = sensors.getTempCByIndex(0);
  // Add the temperature value 
  tempArray[tempIndex] = temperatureC/-10;
  tempIndex++;
  // If the array is full, remove thef irst value
  if (tempIndex >= 51) {
    for (int i = 1; i < 51; i++) {
      tempArray[i - 1] = tempArray[i];
    }
    tempIndex--;
  }
  Serial.print("Current temperature: ");
  Serial.println(temperatureC/-10);
  delay(5000);
}

String getTime() {
  const char* tz = "CET-1CEST,M3.5.0,M10.5.0/3"; // Zeitzone berlin
  configTime(0, 0, tz, "2.de.pool.ntp.org");
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

