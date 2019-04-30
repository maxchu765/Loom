// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include "ADS1115.h"
#include <Wire.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================

struct config_ads1115_t {
  uint16_t delay;
};

struct state_ads1115_t {
  ADS1115 inst_ads1115;
  double radiance;
}; 

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

struct config_ads1115_t config_ads1115;
struct state_ads1115_t  state_ads1115;

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================

bool setup_ads1115();
void package_ads1115(OSCBundle *, char [], uint8_t port); 
void measure_ads1115();

// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any startup for ads1115 that should occur on device startup
// 
// @return  Whether or not sensor initialization was successful
//
//


// currently the mux can't find this sensor, could be due to the address location that is programmed or 
// hardware wiring
bool setup_ads1115()
{                
  //Setup Here
  Serial.println("Initializing ADS1115..."); 
  bool is_setup;
    
  state_ads1115.inst_ads1115 = ADS1115(ADS1115_DEFAULT_ADDRESS);    // creating address location
  state_ads1115.inst_ads1115.initialize();                          // initialize the device
  state_ads1115.inst_ads1115.setMode(ADS1115_MODE_CONTINUOUS);      // set mode
  
  if(state_ads1115.inst_ads1115.testConnection()){                  // test connection 
    is_setup = true;
    config_ads1115.delay = 4000;
    LOOM_DEBUG_Println("Initialized ads1115");
  }
  else{
    is_setup = false;
    LOOM_DEBUG_Println("Failed to initialize ads1115");
  }

    //Serial.print("FINISHED SETTING UP ADS1115\n");
  return is_setup;
}

// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================

// --- PACKAGE ads1115 --- (Multiplexer Version)
//
// Adds OSC Message of most recent sensor readings to a provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//   if I2C multiplexer sensor, then also
// [@param port                  Which port of the multiplexer the device is plugged into]
//

void package_ads1115(OSCBundle *bndl, char packet_header_string[], int port) 
{
    //Serial.print("// --- PACKAGE tmp007 --- (Multiplexer Version)
//
// Adds OSC Message of most recent sensor readings to a provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//   if I2C multiplexer sensor, then also
// [@param port                  Which port of the multiplexer the device is plugged into]
//PACKAGING BUNDLE\n");
  char address_string[255];
  sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/ads1115/data");

  OSCMessage msg = OSCMessage(address_string);

  msg.add("W/m^2").add((int32_t)state_ads1115.radiance);


  
  
  bndl->add(msg);
}

#if is_multiplexer != 1
void package_ads1115(OSCBundle *bndl, char packet_header_string[])
{
  char address_string[255];

  //W/m^2
  sprintf(address_string, "%s%s%s%s", packet_header_string, "/", ads1115_0x48_name, "apogee");
  bndl->add(address_string).add((int32_t)state_ads1115.radiance);
  
}
#endif

// --- MEASURE ads1115 ---
//
// This function uses the ADC to read a voltage differential between two analog inputs.
// It reads in a voltage and will be converted into mV.
//

void measure_ads1115(){
    double mV;
    state_ads1115.inst_ads1115.setGain(ADS1115_MV_0P256);       // set the sensitivity based on sensor output signal
    int counts;
    counts = state_ads1115.inst_ads1115.getConversionP0N1();    // counts up to 16 bits
    mV = counts * state_ads1115.inst_ads1115.getMvPerCount();   // converts to mV
    state_ads1115.radiance = mV * 5;                            // multiply by conversion factor: SP-110 Radiance = 5 W/m^2 per mV
    #if LOOM_DEBUG == 1
      Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
      Serial.print(F("mV ")); Serial.print(mV); Serial.print(F("  "));
      Serial.print(F(" W/m^2: ")); Serial.print(state_ads1115.radiance); Serial.print(F("  "));
    #endif
    delay(config_ads1115.delay);
    
}
