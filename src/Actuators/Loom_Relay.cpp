
#include "Loom_Relay.h"

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_Relay::Loom_Relay( 
		const char*		module_name, 
		byte			pin 
	) 
	: LoomActuator( module_name ) 
{
	this->module_type = ModuleType::Relay;

	this->pin = pin;

	pinMode(pin, OUTPUT);
	on = false;
	digitalWrite(pin, LOW);
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_Relay::Loom_Relay(JsonVariant p)
	: Loom_Relay( EXPAND_ARRAY(p, 2) ) 
{}
// {
	// if (p.size() >= 2) {
	// 	Loom_Relay(p[0], p[1]);
	// } else {
	// 	// Loom_Relay();
	// }
// }


/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_Relay::~Loom_Relay() 
{

}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void Loom_Relay::print_config() 
{
	LoomModule::print_config();
}

/////////////////////////////////////////////////////////////////////
void Loom_Relay::print_state()
{
	print_module_label();
	LPrintln('\t', "Relay ", pin, (on) ? " On" : " Off" );
}

/////////////////////////////////////////////////////////////////////
void Loom_Relay::package(JsonObject json)
{
	package_json(json, module_name, "state", on);
}

// /////////////////////////////////////////////////////////////////////
// bool Loom_Relay::message_route(OSCMessage& msg, int address_offset)
// {
// 	if ( msg.fullMatch( "/SetRelay" , address_offset) ) {
// 		set_relay(msg); return true;
// 	}

// 	return false;
// }

/////////////////////////////////////////////////////////////////////
bool Loom_Relay::cmd_route(JsonObject)
{

}

/////////////////////////////////////////////////////////////////////
// bool Loom_Relay::route_cmd(const JsonObject json)
// {
// 	if ( strcmp(json["module"], module_name) != 0 ) {
// 		return false;
// 	}

// 	JsonArray params = json["params"];

// 	if ( strcmp(json["func"], "set_relay") == 0 ) {
// 		set_relay(params[0]);
// 		return true;
// 	}


// }

// Do an experiment to 


// Decide if route command takes one or multiple commands

// Generalize to use function pointers are arbitrary parameters
// Should:
//	- Check module name
//	- Compare function name against available list of drivers
//	- Call assoicated drivers with all params


// Could generalize drivers to take json only... dont like this much
//   overloading with a json version would be more consistent with constructor approach anyway


/////////////////////////////////////////////////////////////////////
void Loom_Relay::set_relay(bool state)
{
	on = state;
	digitalWrite(pin, (on) ? HIGH : LOW);

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Set relay on pin ", pin, (on) ? " High" : " Low");
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Relay::set_relay(OSCMessage& msg)
{
	set_relay( msg.getInt(0) );
}
