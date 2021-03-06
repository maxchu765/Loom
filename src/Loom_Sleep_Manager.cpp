///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Sleep_Manager.cpp
/// @brief		File for Loom_Sleep_Manager implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Loom_Sleep_Manager.h"
#include "RTC/Loom_RTC.h"

#include <Adafruit_SleepyDog.h>
#include <LowPower.h>


///////////////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager::Loom_Sleep_Manager( 
		const bool		use_LED, 
		const bool		delay_on_wake, 
		const Mode		sleep_mode,
		const byte		power_off_pin
	) 
	: LoomModule( "SleepManager", Type::Sleep_Manager )
	, use_LED(use_LED)
	, delay_on_wake(delay_on_wake)
	, sleep_mode(Mode::STANDBY)
	, power_off_pin(power_off_pin)
{
	pinMode(power_off_pin, OUTPUT);
}

///////////////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager::Loom_Sleep_Manager(JsonArrayConst p)
	: Loom_Sleep_Manager(p[0], p[1], (Mode)(int)p[2], p[3]) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::print_config() const
{
	LoomModule::print_config();
	LPrintln("\tSleep Mode    : ", enum_sleep_mode_string(sleep_mode) );
	LPrintln("\tUse LED       : ", (use_LED) ? "Enabled" : "Disabled" );
	LPrintln("\tDelay on Wake : ", (delay_on_wake) ? "Enabled" : "Disabled" );
	LPrintln("\t\t Interrupt? ", interrupt_manager ? "+" : "-");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::link_device_manager(LoomManager* LM)
{
	LoomModule::link_device_manager(LM);

	if ( LM ){

		// Set manager's sleep manager 
		LM->sleep_manager = this;

		// Link to interrupt manager
		auto interrupt_manager = LM->get_interrupt_manager(); 
		if ( interrupt_manager ) {
			link_interrupt_manager(interrupt_manager);
			interrupt_manager->link_sleep_manager(this);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::powerDown()
{
	// Call manager to coordinate modules saving to flash

// to be implemented

	// Use manager to power all modules off
	if (device_manager != nullptr) {
		device_manager->power_down();
	}

	LPrintln("Powering Off");
	digitalWrite(power_off_pin, HIGH);
	delay(50); // Sometimes the board takes several milliseconds to fully power off
	LPrintln("This should not be printed");
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
	// Use manager to power all modules off
	if (device_manager != nullptr) {
		device_manager->power_down();
	}

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

	// Use manager to power all active modules up
	if (device_manager != nullptr) {
		device_manager->power_up();
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
const char* Loom_Sleep_Manager::enum_sleep_mode_string(const Mode m)
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

