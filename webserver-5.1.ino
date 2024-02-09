#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char* ssid = "redminote9s";
const char* password = "PraktikuM!";

unsigned long previousMillis = 0; // The time of the last update
unsigned long currentMillis; // The current time

ESP8266WebServer server(80);
OneWire oneWire(D2);
DallasTemperature sensors(&oneWire);

const char* tempArray[51][2]; // increase the size to hold the temperature and time strings
int i = 0;
String prev_time_str;
String content = "";
String getTime() {
  const char* tz = "CET-1CEST,M3.5.0,M10.5.0/3"; // Zeitzone berlin
  configTime(0, 0, tz, "2.de.pool.ntp.org");
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  char time[20];
  strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return time;
}
float getTemperature() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0)/-10;
  return temperatureC;
}

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
  
  sensors.begin();

  server.on("/", []() {
    String curr_time_str = getTime();
    float curr_temp_val = getTemperature();
    char curr_time[20];
    strncpy(curr_time, curr_time_str.c_str(), sizeof(curr_time));
    char curr_temp[6];
    dtostrf(curr_temp_val, 1, 2, curr_temp);
    char html[100];
    snprintf(html, sizeof(html), "<html><p>Current time: %s</p><p>Current temperature: %s C</p></html>", curr_time, curr_temp);
    server.send(200, "text/html", html);
  });
  server.on("/api/json", []() {
    String curr_time_str = getTime();
    float curr_temp_val = getTemperature();
    char curr_time[20];
    strncpy(curr_time, curr_time_str.c_str(), sizeof(curr_time));
    char curr_temp[6];
    dtostrf(curr_temp_val, 1, 2, curr_temp);
    char json[100];
    snprintf(json, sizeof(json), "{\"Hello World\":\"Current time: %s\",\"Current temperature\":\"%s C\"}", curr_time, curr_temp);
    server.send(200, "application/json", json);
  });
  server.on("/api/download", []() {
    server.sendHeader("Content-Disposition", "attachment; filename=helloworld.txt");
    server.send(200, "text/plain", content);
    
  });
    server.on("/api/home", []() {
    String html_1 = "Hello everything link: <a href='http://192.168.223.247/api/json'><button>Click me</button></a>";
    String html_2 = "<br>download data link: <a href='http://192.168.223.247/api/download'><button>Click me for download</button></a>";
    server.send(200, "text/html", html_1 + html_2);
  });
  server.begin();
}

void loop() {
    server.handleClient();
    String curr_time_str = getTime();
    float curr_temp_val = getTemperature();
    char curr_time[20];
    strncpy(curr_time, curr_time_str.c_str(), sizeof(curr_time));
    char curr_temp[6];
    dtostrf(curr_temp_val, 1, 2, curr_temp);
    String curr_time_str_obj = String(curr_time);// Convert the curr_time array to a String object

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 10000) {
      previousMillis = currentMillis;
      content += ("\n " + curr_time_str_obj + "  :  " + String(curr_temp)  + " C");
      i++;
    }
    if (i >= 51){//reset i if > 51; reset content
        content = "";
        i = 0;
    }  
}
 
