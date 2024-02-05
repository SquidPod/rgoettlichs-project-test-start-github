#include<ESP8266WiFi.h>

const char* ssid= "Voucher";
const char* password = "";


void setup() {
  // put your setup code here, to run once:
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.begin(9600);

  while(WiFi.status() != WL_CONNECTED ){
    delay(500);
    Serial.print(".");

  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP-Adress: ");
  Serial.println(WiFi.localIP());



}

void loop() {
  // put your main code here, to run repeatedly:

}
