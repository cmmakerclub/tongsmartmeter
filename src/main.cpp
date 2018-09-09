
#include <CMMC_Legend.h>
#include <CMMC_Sensor.h>

#include "modules/WiFiModule.h"
#include "modules/MqttModule.h"
#include "modules/SensorModule.h"
#include "modules/ConfigButtonModule.h"

const char* MEOBOT_VERSION = "1.4";

CMMC_Legend os; 
SensorModule *sensorModule;
WiFiModule* wifiModule;


void setup()
{ 
  wifiModule = new WiFiModule();
  sensorModule = new SensorModule();

  os.addModule(wifiModule); 
  os.addModule(sensorModule);
  os.addModule(new MqttModule()); 

  os.setup();
  Serial.printf("APP VERSION: %s\r\n", LEGEND_APP_VERSION);
  Serial.printf("MEOBOT VERSION: %s\r\n", MEOBOT_VERSION);
}

void loop()
{
  os.run();
}