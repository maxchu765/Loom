// Note that the PCF8523 is different than the DS3231 in a few aspects
// - Alarms only have minute (rather than second) precision
//		Any alarms set for the current minute will be delayed until the start of the next minute
// - The interrupt is a constant low signal, rather than a squareware
//		No need to detach the interrupts, just clear the alarms

#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

LoomManager Loom{};


#define ALARM_PIN 6

volatile bool alarmFlag = false;
volatile int count = 0;
void alarmISR() { 
	Loom.InterruptManager().get_RTC_module()->clear_alarms();
	count++;
	alarmFlag = true;
}


void setup() 
{ 
	Loom.begin_LED();
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();

	Loom.InterruptManager().register_ISR(ALARM_PIN, alarmISR, LOW, ISR_Type::IMMEDIATE);
	Loom.InterruptManager().RTC_alarm_duration(TimeSpan(10));
	digitalWrite(LED_BUILTIN, LOW);

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	if (alarmFlag) {
		digitalWrite(LED_BUILTIN, HIGH);
		LPrintln("Alarm triggered, resetting alarm");
		delay(1000);
		
		Loom.InterruptManager().RTC_alarm_duration(TimeSpan(10)); 

		digitalWrite(LED_BUILTIN, LOW);
		alarmFlag = false;
	}

	LPrintln("Count: ", count);
	Loom.PCF8523().print_time();
	delay(1000);

}