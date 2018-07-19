// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <OSCBundle.h> //all LOOM devices rely on the Arduino OSC library in some way


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
// Packet header creation macro
#define STR_(x) #x                // Helper function
#define STR(x) STR_(x)            // To concatenate a predefined number to a string literal, use STR(x)
#define PacketHeaderString STR(/) FAMILY STR(/) DEVICE // Results in a single string, i.e. /LOOM/Device. the full prefix sent to this device should be /LOOM/Device#, but the number is parsed in the OSC bundle routing function

#define VBATPIN A7                // Pin to check for battery voltage

// If using channels
#if CHANNEL >= 1 && CHANNEL <= 8
	#define INIT_INST CHANNEL
	#define INIT_PORT 9440+CHANNEL
#endif


// --- WiFi UDP Ports ---
#if is_wifi == 1
	#define COMMON_PORT     9440	// Expected by Max to be 9440, don't change unless using custom Max patches
#endif


// --- Automatically Set Device Name ---
#if AUTO_NAME == 1
	// Make sure only one device type is enabled
	#if ( (is_ishield) + (num_servos > 0) + (num_steppers > 0) + (is_relay) + (is_decagon) + (is_multiplexer) ) > 1
		autoname_device_type_error	// this will force an error if too many sensor/actuator were defined (needs to be = 1 for autoname to work)
	#endif

	// Automatically set device name
	#if   is_ishield 
		#define DEVICE "Ishield"
	#elif num_servos > 0
		#define DEVICE "ServoShield"
	#elif num_steppers > 0
		#define DEVICE "Stepper"
	#elif is_relay
		#define DEVICE "RelayShield"
	#elif is_decagon
		#define DEVICE "Decagon"
	#elif is_multiplexer
		#define DEVICE "MuxShield"
	#elif is_sapflow
		#define DEVICE "Sapflow"
	#else
		#define DEVICE "Unknown"
	#endif
#else
	// --- Custom Device Identification --- 
	#define DEVICE CUSTOM_NAME // The device name (can be changed), used only if not using automatic device name
#endif




// Enumerate possible platform types
enum CommPlatform {
	WIFI,
	LORA,
	NRF
	// CELLULAR  
};

enum LogPlatform {
	SDCARD,
	PUSHINGBOX,
	// ADAFRUITIO,
	SERIAL_MON
};

// Macros for printing to Serial iff Loom Debug is enabled
#define LOOM_DEBUG_Print(X)          (LOOM_DEBUG==0) ? :  Serial.print(X)
#define LOOM_DEBUG_Println(X)        (LOOM_DEBUG==0) ? :  Serial.println(X)
#define LOOM_DEBUG_Print2(X,Y)       LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y)
#define LOOM_DEBUG_Println2(X,Y)     LOOM_DEBUG_Print(X); LOOM_DEBUG_Println(Y)
#define LOOM_DEBUG_Print3(X,Y,Z)     LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Print(Z)
#define LOOM_DEBUG_Println3(X,Y,Z)   LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Println(Z)
#define LOOM_DEBUG_Print4(W,X,Y,Z)   LOOM_DEBUG_Print(W); LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Print(Z)
#define LOOM_DEBUG_Println4(W,X,Y,Z) LOOM_DEBUG_Print(W); LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Println(Z)


void receive_bundle(OSCBundle *bndl, CommPlatform platform);
void process_bundle(OSCBundle *bndl);
void measure_sensors();
void package_data(OSCBundle *send_bndl);
void send_bundle(OSCBundle *send_bndl, CommPlatform platform);
void log_bundle(OSCBundle *log_bndl, LogPlatform, char* file);
void log_bundle(OSCBundle *log_bndl, LogPlatform);


// ================================================================
// ===                 COMMON GLOBAL VARIABLES                  ===
// ================================================================
int           led = LED_BUILTIN;              // LED pin number
volatile bool ledState = LOW;                 // State of LED
float         vbat = 3.3;                     // Place to save measured battery voltage (3.3V max)
char          packetBuffer[255];              // Buffer to hold incoming packet
char          ReplyBuffer[] = "acknowledged"; // A string to send back
OSCErrorCode  error;                          // Hold errors from OSC
uint32_t      button_timer;                   // For time button has been held
int 		  button_state;
char          addressString[255];

char global_packet_header_string[80]; // Sometimes functions need to access the header string but are declared before loom_flash.h is included


//---------------------------------------------------------------------------
// MEMORY TYPE: M0 uses flash (MEM_TYPE = 0), 32u4 uses EEPROM (MEM_TYPE = 1)
//---------------------------------------------------------------------------
#define MEM_FLASH 0
#define MEM_EEPROM 1  

// Still experimental
#ifdef __SAMD21G18A__
	#define is_m0
	#define MEM_TYPE MEM_FLASH
#endif
#ifdef __AVR_ATmega32U4__
	#define is_32u4
	#define MEM_TYPE MEM_EEPROM
#endif

#if is_relay == 0
	#define button BUTTON_PIN               // Using on-board button, specify attached pin, transmitting
#endif
#ifdef is_sleep_period
	#include <Adafruit_SleepyDog.h> // This must be included if you are transmitting at timed intervals
#endif

#ifdef is_sleep_interrupt
	#include <LowPower.h>           // Include this if transmitting on pin interrupt
#endif

// Using Config defines, check which headers to include
#if LOOM_DEBUG == 1
	#include <SPI.h>
#endif


// Prototypes of functions from loom_flash.h, loom_common.h, and loom_OSC_translator.h
// That are referenced by device .h files
void read_non_volatile();
void write_non_volatile();
void convert_OSC_string_to_bundle(char *osc_string, OSCBundle* bndl);
void convert_OSC_bundle_to_string(OSCBundle *bndl, char *osc_string);
String get_data_value(OSCMessage* msg, int pos);
#if LOOM_DEBUG == 1
	void print_bundle(OSCBundle *bndl);
#endif
int get_bundle_bytes(OSCBundle *bndl); 			



// ================================================================ 
// ===                  INCLUDE DEVICE FILES                    === 
// ================================================================
#include "loom_translator.h"

#if is_fona == 1
	#include "loom_fona.h"
#endif
#if is_ethernet == 1
	#include "loom_ethernet.h"
#endif
#if is_wifi == 1
	#include "loom_wifi.h"
#endif
#if is_pushingbox == 1
	// #include "loom_ethernet.h"
	#include "loom_pushingbox.h"
#endif
#if is_lora == 1
	#include "loom_lora.h"
#endif
#if is_nrf == 1	
	#include "loom_nrf.h"
#endif


// #if num_analog > 0     
// always include, because battery uses analog read, 
// numbers would change due to 12bit ADC if this was removed, 
// it is a small file anyway
	#include "loom_analogIn.h"
// #endif
#if is_neopixel == 1
	#include "loom_neopixel.h"
#endif
#if num_servos > 0
	#include "loom_servo.h"
#endif
#if num_steppers > 0
	#include "loom_stepper.h"
#endif
#if is_relay == 1
	#include "loom_relay.h"
#endif
#if is_mpu6050 == 1
	#include "loom_mpu6050.h"
#endif
#if is_max31856 == 1
	#include "loom_max31856.h"
#endif
#if is_tsl2591 == 1
	#include "loom_tsl2591.h"
#endif
#if is_fxos8700 == 1
	#include "loom_fxos8700.h"
#endif
#if is_fxas21002 == 1
	#include "loom_fxas21002.h"
#endif
#if is_zxgesturesensor == 1
	#include "loom_zxgesturesensor.h"
#endif
#if is_sht31d == 1
	#include "loom_sht31d.h"
#endif
#if is_mb1232 == 1
	#include "loom_mb1232.h"
#endif
#if is_decagon == 1
	#include "loom_decagon.h"
#endif
#if is_multiplexer == 1
	#include "loom_tca9548a.h"
#endif
#if is_rtc == 1
	#include "loom_rtc.h"
#endif
#if is_sd == 1
	#include "loom_sd.h"
#endif
#if is_adafruitio == 1
	#include "loom_adafruit_io.h"
#endif
#if is_sapflow == 1
	#include "loom_sapflowmeter.h"
#endif



// Files of functions that are not specific to sensors / actuators
#include "loom_flash.h"
#include "loom_common.h"  		// These may refer to functions in above headers
#include "loom_msg_router.h"
#include "loom_interface.h"









