#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Redmi Note 9s";
const char* password = "hallohallo";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    String time = getTime();
    String html = "<html><body><h1>Real-time</h1><p>" + time + "</p></body></html>";
    server.send(200, "text/html", html);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}

String getTime() {
  configTime(0, 0, "pool.ntp.org");
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char time[20];
  strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", &timeinfo);

  return String(time);
}

