#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// alle include's
const char* ssid = "redminote9s";
const char* password = "PraktikuM!";
//alle const char's
ESP8266WebServer server(80);
OneWire oneWire(D2);
DallasTemperature sensors(&oneWire);
//alle normalen variablen
//void setup
void setup() {
  //alles begin, server später
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  //sagen wenn es mit WiFi verbunden ist
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    Serial.print(".");
  }
  //sagen, dass es mit WiFi verbunden ist:
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //server.on's -> adressen im  webserver selber(3 an der zahl: startseite[/]; kurze daten zusammengefassst[ip/api/hello]; txt- datei mit diesen daten )


  server.on("/", []() {
    String time = getTime();
    float temperatureC = getTemperature();
    String html = time + "</p><p>Current temperature: " + String(temperatureC) + " C</p>";
    server.send(200, "text/html", html);
  });

  server.on("/api/hello", []() {
    String time = getTime();
    float temperatureC = getTemperature();
    String json = "{\"message\":\"Hello World\",\"time\":\"" + time + "\",\"temperature\":\"" + String(temperatureC) + "\"}";
    server.send(200, "application/json", json);
  });

  server.on("/api/helloworld", []() {
    String time = getTime();
    float temperatureC = getTemperature();
    String content = "Hello World\nCurrent time: " + time + "\nCurrent temperature: " + String(temperatureC) + " C";
    server.sendHeader("Content-Disposition", "attachment; filename=helloworld.txt");
    server.send(200, "text/plain", content);
  });

  //server start

  server.begin();
}

//void loop-> alle funjktionen, die permament laufne müssen->server aktiv bleiben

void loop() {
  server.handleClient();
}

//zeit bekommen(gettime) ich bin durch ein tutorial auf diese einfache weise gekommen zeit abzurufen-> dieser teil ist im prinzip copy-pasted

String getTime() {
  configTime(0, 0, "pool.ntp.org");
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char time[20];
  strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", &timeinfo);

  return String(time);
}

//das gleiche wie im "alten projekt"

float getTemperature() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0)/-10;
  return temperatureC;
}


