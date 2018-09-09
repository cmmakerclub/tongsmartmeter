#include <CMMC_Module.h>

#ifndef CMMC_SensorModule_MODULE_H
#define CMMC_SensorModule_MODULE_H 

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <CMMC_Sensor.h>
#include <CMMC_Interval.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADS1015.h> 

const int MAX_ARRAY = 3;
class SensorModule: public CMMC_Module {
  public:
    float soil_moisture_percent = 0.0;
    void config(CMMC_System *os, AsyncWebServer* server); 
    void setup();
    void loop(); 
    void isLongPressed(); 
    void configLoop();
    String getTemperatureString();
    String getHumidityString();
    float getTemperature();
    float getHumidity();
    float getAnalog(int );
    int getCurrent();
    int soil_enable = false; 
  protected:
    void configWebServer();
  private:
    //Adafruit_BME280 *bme;
    CMMC_SENSOR_DATA_T data1; 
    CMMC_Interval interval;
    
    float temp_array[MAX_ARRAY] = { 0.0 };
    float humid_array[MAX_ARRAY] = { 0.0 };
    float pressure_array[MAX_ARRAY] = { 0.0 }; 
    float adc0_array[MAX_ARRAY] = { 0 };

    float readCurrent(int PIN);
    int determineVQ(int PIN);

    float _temperature;
    float _humidity;
    float _pressure; 
    float _adc0;
    int soil_max;
    int soil_min;
    int soil_moisture;

    unsigned long counter = 0;
    Adafruit_ADS1115 *ads;
    
  const int currentPin = A0;
const unsigned long sampleTime = 200000UL;                           // sample over 100ms, it is an exact number of cycles for both 50Hz and 60Hz mains
const unsigned long numSamples = 250UL;                               // choose the number of samples to divide sampleTime exactly, but low enough for the ADC to keep up
const unsigned long sampleInterval = sampleTime/numSamples;  // the sampling interval, must be longer than then ADC conversion time
//const int adc_zero = 522;                                                     // relative digital zero of the arudino input from ACS712 (could make this a variable and auto-adjust it)
int adc_zero; 

};

#endif