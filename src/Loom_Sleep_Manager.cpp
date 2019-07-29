
#include "Loom_Sleep_Manager.h"
#include "RTC/Loom_RTC.h"

#include <Adafruit_SleepyDog.h>
#include <LowPower.h>


///////////////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager::Loom_Sleep_Manager( 
		const char*		module_name, 
		bool			use_LED, 
		bool 			delay_on_wake, 
		Mode			sleep_mode,
		byte			power_off_pin
	) : LoomModule( module_name )
{
	this->module_type = LoomModule::Type::Sleep_Manager;

	this->use_LED		= use_LED;
	this->delay_on_wake	= delay_on_wake;
	this->sleep_mode	= Mode::STANDBY;

	this->power_off_pin	= power_off_pin;
	pinMode(power_off_pin, OUTPUT);
}

///////////////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager::Loom_Sleep_Manager(JsonArrayConst p)
	: Loom_Sleep_Manager(p[0], p[1], p[2], (Mode)(int)p[3], p[4]) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::link_interrupt_manager(Loom_Interrupt_Manager* IM)
{
	interrupt_manager = IM;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::print_config()
{
	LoomModule::print_config();
	LPrintln('\t', "Sleep Mode          : ", enum_sleep_mode_string(sleep_mode) );
	LPrintln('\t', "Use LED             : ", (use_LED) ? "Enabled" : "Disabled" );
	LPrintln('\t', "Delay on Wake       : ", (delay_on_wake) ? "Enabled" : "Disabled" );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::print_state()
{
	LoomModule::print_state();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::powerDown()
{
	// Call manager to coordinate modules saving to flash
	// and running any individual powerDown methods

	LPrintln("Powering Off");
	digitalWrite(power_off_pin, HIGH);
	delay(50); // Sometimes the board takes several milliseconds to fully power off
	LPrintln("This should not be printed");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::set_sleep_mode(Mode mode)
{
	sleep_mode = mode;
}

///////////////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager::Mode Loom_Sleep_Manager::get_sleep_mode()
{
	return sleep_mode;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Sleep_Manager::sleep()
{
		pre_sleep();

		LowPower.standby();
		// This is where programs waits until waking

		post_sleep();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::pre_sleep()
{
	LPrintln("\nEntering STANDBY");
	delay(50);
	Serial.end();
	USBDevice.detach();

	digitalWrite(LED_BUILTIN, LOW);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::post_sleep()
{
	// Prevent double trigger of alarm interrupt
	if (interrupt_manager) {
		interrupt_manager->get_RTC_module()->clear_alarms();
	}

	if (use_LED) {
		digitalWrite(LED_BUILTIN, HIGH);
	}

	#if LOOM_DEBUG == 1
		USBDevice.attach();
		Serial.begin(115200);
		if (delay_on_wake) {
			// Give user 5s to reopen Serial monitor!
			// Note that the serial may still take a few seconds 
			// to fully setup after the LED turns on
			delay(5000); 
		}
		LPrintln("WAKE");
	#endif

	// Set wake time

}

///////////////////////////////////////////////////////////////////////////////
const char* Loom_Sleep_Manager::enum_sleep_mode_string(Mode m)
{
	switch(m) {
		case Mode::IDLE				: return "Idle";
		case Mode::STANDBY			: return "Standby";
		case Mode::SLEEPYDOG		: return "SleepyDog";
		case Mode::OPENS_LOWPOWER	: return "OPEnS_Lowpower";
		default : return "";
	}
}

///////////////////////////////////////////////////////////////////////////////

