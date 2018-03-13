#include <ArduinoJson.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  while (!Serial) continue;
}

void loop() {
  // put your main code here, to run repeatedly:
  StaticJsonBuffer<200> jsonBuffer;
  char json[200] = "{}";
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
  }
  root["TEMP"] = random(300);
  root["HUMIDITY"] = random(300); 
  Serial.println();
  if(Serial.available()){
    root["MSG"] = Serial.readString();
  }
  root.printTo(Serial); 
  delay(10000); 
}
