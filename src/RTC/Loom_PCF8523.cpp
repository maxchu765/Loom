
#include "Loom_PCF8523.h"


///////////////////////////////////////////////////////////////////////////////
Loom_PCF8523::Loom_PCF8523(	
		TimeZone		timezone,
		bool			use_utc_time
	)
	: LoomRTC( "PCF8523", Type::PCF8523, timezone, use_utc_time )
{
	init();
	clear_alarms();
}

///////////////////////////////////////////////////////////////////////////////
Loom_PCF8523::Loom_PCF8523(JsonArrayConst p)
	: Loom_PCF8523( (TimeZone)(int)p[0], p[1] ) {}

///////////////////////////////////////////////////////////////////////////////
bool Loom_PCF8523::_begin()
{
	rtc_inst.begin();
	rtc_inst.stop_32768_clkout();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_PCF8523::print_config()
{
	LoomRTC::print_config();
	// will print out alarm info
}

///////////////////////////////////////////////////////////////////////////////
void Loom_PCF8523::set_alarm(DateTime time)
{
	// Don't set alarm for current minute
	DateTime tmp = (now().minute() != time.minute()) ? time : time+TimeSpan(60);

	rtc_inst.set_alarm(tmp.day(), tmp.hour(), tmp.minute() );
	rtc_inst.enable_alarm(true);
}

///////////////////////////////////////////////////////////////////////////////