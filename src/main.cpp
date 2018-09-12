
#include <CMMC_Legend.h>

#include "modules/WiFiModule.h"
#include "modules/MqttModule.h"
#include "modules/SensorModule.h"
#include "modules/ConfigButtonModule.h"

const char* MEOBOT_VERSION = "1.4";

CMMC_Legend os; 

WiFiModule* wifiModule; 
MqttModule *mqttModule;
SensorModule *sensorModule;

void setup()
{ 
  pinMode(15, OUTPUT);
  pinMode(13, INPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(15, HIGH); 
  digitalWrite(2, LOW); 


  mqttModule = new MqttModule();
  wifiModule = new WiFiModule();
  sensorModule = new SensorModule();

  os.addModule(wifiModule); 
  os.addModule(sensorModule); 
  os.addModule(mqttModule); 

  os.setup();
  Serial.printf("APP VERSION: %s\r\n", LEGEND_APP_VERSION); 
}

void loop()
{
  os.run();
}