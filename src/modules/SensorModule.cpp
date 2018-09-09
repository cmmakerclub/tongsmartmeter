#include "SensorModule.h"
void swap(float &a, float &b)
{
  int t = a;
  a = b;
  b = t;
}

int partition(float *arr, const int left, const int right)
{
  const int mid = left + (right - left) / 2;
  const int pivot = arr[mid];
  // move the mid point value to the front.
  swap(arr[mid], arr[left]);
  int i = left + 1;
  int j = right;
  while (i <= j)
  {
    while (i <= j && arr[i] <= pivot)
    {
      i++;
    }

    while (i <= j && arr[j] > pivot)
    {
      j--;
    }

    if (i < j)
    {
      swap(arr[i], arr[j]);
    }
  }

  swap(arr[i - 1], arr[left]);
  return i - 1;
}

void quickSort(float *arr, const int left, const int right)
{
  if (left >= right)
  {
    return;
  }

  int part = partition(arr, left, right);

  quickSort(arr, left, part - 1);
  quickSort(arr, part + 1, right);
}

int median(float arr[], int maxValues)
{
  quickSort(arr, 0, maxValues - 1);
  return arr[maxValues / 2];
}

void SensorModule::configLoop()
{
}

void SensorModule::config(CMMC_System *os, AsyncWebServer *server)
{
  strcpy(this->path, "/api/sensors");
  this->_serverPtr = server;
  this->_managerPtr = new CMMC_ConfigManager("/sensors.json");
  this->_managerPtr->init();
  this->_managerPtr->load_config([&](JsonObject * root, const char *content) {
    if (root == NULL)
    {
      Serial.print("sensors.json failed. >");
      Serial.println(content);
      return;
    }
    else {
      
    }
  });
  this->configWebServer();
}

void SensorModule::configWebServer()
{
  _serverPtr->on(this->path, HTTP_POST, [&](AsyncWebServerRequest *request) {
    String output = saveConfig(request, this->_managerPtr);
    request->send(200, "application/json", output);
  });
}


float SensorModule::getAnalog(int slot)
{
  return _adc0;
}

void SensorModule::setup()
{
  adc_zero = determineVQ(currentPin); //Quiscent output voltage - the average voltage ACS712 shows with no load (0 A)
   pinMode(15, OUTPUT);
   digitalWrite(15, HIGH);
}

void SensorModule::loop()
{

  interval.every_ms(2000, [&]() {
    int idx = counter % MAX_ARRAY;
    Serial.print("ACS712@A2:");Serial.print(readCurrent(currentPin),1);Serial.println(" mA");
        
        


    // int16_t adc1 = ads->readADC_SingleEnded(1);
    // int16_t adc2 = ads->readADC_SingleEnded(2);
    // int16_t adc3 = ads->readADC_SingleEnded(3);

    // Serial.printf("adc0 = %d\r\n", adc0);
    // Serial.printf("adc1 = %d\r\n", adc1);
    // Serial.printf("adc2 = %d\r\n", adc2);
    // Serial.printf("adc3 = %d\r\n", adc3);

    //temp_array[idx] = bme->readTemperature();
    //humid_array[idx] = bme->readHumidity();
    if (counter < MAX_ARRAY)
    {
      _temperature = median(temp_array, idx + 1);
      _humidity = median(humid_array, idx + 1);
      _pressure = median(pressure_array, idx + 1);
      _adc0 = median(adc0_array, idx + 1);
    }
    else
    {
      _temperature = median(temp_array, MAX_ARRAY);
      _humidity = median(humid_array, MAX_ARRAY);
      _pressure = median(pressure_array, MAX_ARRAY);
      _adc0 = median(adc0_array, MAX_ARRAY);
    }
    
    float a0_percent = map(_adc0, soil_min, soil_max, 100, 0);
    if (soil_enable) {
      soil_moisture_percent = a0_percent; 
      if (a0_percent <= soil_moisture) {
        digitalWrite(2, HIGH); 
      } 
      else {
        digitalWrite(2, LOW); 
      }
    }
    Serial.printf("temp=%.2f humid=%.2f, adc0=%.2f, %f%%\r\n", 
      this->_temperature, this->_humidity, this->_adc0, soil_moisture_percent); 
    counter++;
  });
}

String SensorModule::getTemperatureString()
{
  char buffer[10];
  sprintf(buffer, "%.1f", _temperature);
  return String(buffer);
}

String SensorModule::getHumidityString()
{
  return String((int)_humidity);
}

float SensorModule::getTemperature()
{
  return _temperature;
}

float SensorModule::getHumidity()
{
  return _humidity;
}
int SensorModule::determineVQ(int PIN) {
  Serial.print("estimating avg. quiscent voltage:");
  long VQ = 0;
  //read 5000 samples to stabilise value
  for (int i=0; i<5000; i++) {
    VQ += analogRead(PIN);
    delay(1);//depends on sampling (on filter capacitor), can be 1/80000 (80kHz) max.
  }
  VQ /= 5000;
  Serial.print(map(VQ, 0, 1023, 0, 5000));Serial.println(" mV");
  return int(VQ);
}

float SensorModule::readCurrent(int PIN)
{
  unsigned long currentAcc = 0;
  unsigned int count = 0;
  unsigned long prevMicros = micros() - sampleInterval ;
  while (count < numSamples)
  {
    if (micros() - prevMicros >= sampleInterval)
    {
      int adc_raw = analogRead(currentPin) - adc_zero;
      currentAcc += (unsigned long)(adc_raw * adc_raw);
      ++count;
      prevMicros += sampleInterval;
    }
  }
  float rms = sqrt((float)currentAcc/(float)numSamples) * (27.027 / 1024.0);
  return rms;
  //Serial.println(rms);
}
int SensorModule::getCurrent(){
  return readCurrent(currentPin);
}