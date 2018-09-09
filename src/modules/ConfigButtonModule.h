#include <CMMC_Module.h>

#ifndef CMMC_ConfigButtonModule_MODULE_H
#define CMMC_ConfigButtonModule_MODULE_H 

class ConfigButtonModule: public CMMC_Module {
  public:
    void config(CMMC_System *os, AsyncWebServer* server); 
    void configSetup();
    void setup();
    void loop(); 
    void isLongPressed(); 
    void configLoop();
  protected:
    
  private:

};

#endif