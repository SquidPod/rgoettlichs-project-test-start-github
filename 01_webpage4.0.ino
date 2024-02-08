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

float tempArray[51][1];
int i,tempIndex = 0;
String curr_time;
float curr_temp;

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



void setup() {
    server.begin();
    sensors.begin();
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

  //server.on("/", []() {
    //curr_time = getTime();
   // curr_temp = getTemperature();
   // String html = time + "</p><p>Current temperature: " + String(temperatureC) + " C</p>";
   // server.send(200, "text/html", html);
 // });
 while (tempIndex<51) {

   curr_time = getTime();
    curr_temp = getTemperature();
  // Add the temperature value 
  tempArray[tempIndex][0] = curr_temp/-10;
  tempArray[tempIndex][1] = curr_time;
  
  tempIndex++;
  // If the array is full, remove thef irst value
  if (tempIndex >= 51) {
    for (i = 1; i < 51; i++) {
      tempArray[i - 1][0] = tempArray[i][0];
      tempArray[i - 1][1] = tempArray[i][1];
    }
    tempIndex--;
   }
  server.on("/api/hello", []() {
    String json = "{\"Hello World\":\"Current time: " + String(curr_time) + "\",\"Current temperature\":\"" + String(curr_temp) + " C\"}";
    server.send(200, "application/json", json);
  });
 
  server.on("/api/helloworld", []() {
      for (i = 0; i < tempIndex; i++) {
        content += "\n " + String(tempArray[i][1]) + "  :  " + String(tempArray[i][0]) + " C" ;
      }
      server.sendHeader("Content-Disposition", "attachment; filename=helloworld.txt");
      server.send(200, "text/plain " , content);
  });
 }
 
  server.on("/api/start", []() {
    String html_1 = "Hello everything link: <a href='http://192.168.56.247/api/hello'><button>Click me</button></a>";
    String html_2 = "<br>download data link: <a href='http://192.168.56.247/api/helloworld'><button>Click me</button></a>";
    server.send(200, "text/html", html_1 + html_2);
  });

}
//void loop ist pflicht, und ist selberein infinit loop, deswegen würde ich sagen, dass der infinit loop in setup hier rein kommt
//



/* ausserdem müssen die server.on's  alle ausserhalb des while loops sein das heisst diese bleiben in setup
danach müsste noch content definiert werden als content = "";
das letzte wäre noch das problem, wenn man die while schleife hat sagt man in line 68, dass man curr_time in nen float umwandelt-> string in float und das geht nicht, deswegen hätte ic 2 eindimensienale arrays gemacht damit temparray nen float und timearray nen string sein kann so von
der theorie her.

leider habe ich keine ahenung wie ich das umsetzten kann(alles)*/



