#include <OneWire.h>
#include<DallasTemperature.h>

const int datenkabeld2 = 4;

OneWire oneWire(datenkabeld2);

DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);

  sensors.begin();
}


void loop(){
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(1)/-10;
  Serial.print(temperatureC);
  Serial.println("°C");
  delay(5500);
}

