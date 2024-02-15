#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

String response_data;
const char* ssid = "BARAMUN-7IRUH9L 6588";
const char* password = "12345678";


const char* serverName = "http://172.21.129.202/esp8266_project/test_data2.php";
WiFiClient client;
HTTPClient http;
ESP8266WebServer server(80);


void setup() {
  
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());



  server.on("/", []() {
    char html[100];
    snprintf(html, sizeof(html), "<html><p> it works! :) </p></html>");
    server.send(200, "text/html", html);
    server.sendHeader("Content-Disposition", "attachment; filename=helloworld.txt");
    server.send(200, "text/plain", response_data);
  });

  server.on("/api/download", []() {
    server.sendHeader("Content-Disposition", "attachment; filename=helloworld.txt");
    server.send(200, "text/plain", response_data);
    
  });
  server.on("/api/home", []() {
    String html_2 = "<br>download data link: <a href='http://192.168.137.157/api/download'><button>Click me for download</button></a>";
    server.send(200, "text/html",html_2);
  });

  server.begin();

}


void loop() {
   
  if (WiFi.status() == WL_CONNECTED) {
    while (true) {
      server.handleClient();

      String url = serverName;
      
      http.begin(client, url);

      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        response_data = http.getString();
      } else {
        Serial.print("Error on sending GET: ");
        Serial.println(httpResponseCode);
      }

      http.end();
      
      delay(30000); 
    }
  } 

}