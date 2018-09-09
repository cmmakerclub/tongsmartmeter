#include "ConfigButtonModule.h"

 

void ConfigButtonModule::isLongPressed() {
  if (digitalRead(13) == LOW) {
    while(digitalRead(13) == LOW) {
      delay(10); 
    }
    SPIFFS.remove("/enabled");
    digitalWrite(0, LOW);
    delay(2000);
    ESP.restart();
  }
}

void ConfigButtonModule::configLoop() {
  if (digitalRead(13) == LOW) {
    while(digitalRead(13) == LOW) {
      delay(10); 
    }
    File f = SPIFFS.open("/enabled", "a+");
    f.close();
    digitalWrite(0, LOW);
    delay(100);
    ESP.restart();
  }
}

void ConfigButtonModule::configSetup() {
    pinMode(13,INPUT_PULLUP);
}

void ConfigButtonModule::config(CMMC_System *os, AsyncWebServer *server)
{
}


void ConfigButtonModule::setup()
{
    pinMode(13,INPUT_PULLUP);

}

void ConfigButtonModule::loop() {
    isLongPressed();
}

