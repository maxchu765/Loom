
#include "Loom_Module.h"


///////////////////////////////////////////////////////////////////////////////
LoomModule::LoomModule( const char* set_module_name )
	: module_name_base(set_module_name)
	, module_name(module_name_base.c_str())
{
	active 				= true;
	print_debug 		= true;
	print_verbosity 	= Verbosity::V_LOW;
	package_verbosity 	= Verbosity::V_LOW;
	
	this->device_manager = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
LoomModule::LoomModule( ) 
	: LoomModule("Unknown") 
{}

///////////////////////////////////////////////////////////////////////////////
LoomModule::Type LoomModule::get_module_type()
{
	return module_type;
}

///////////////////////////////////////////////////////////////////////////////
LoomManager* LoomModule::get_device_manager()
{
	return device_manager;
}

///////////////////////////////////////////////////////////////////////////////
void LoomModule::link_device_manager(LoomManager* LM)
{
	if (LM == nullptr) return;
	device_manager = LM;
}

///////////////////////////////////////////////////////////////////////////////
void LoomModule::print_module_label()
{
	LPrint("[", module_name, "] ");
}

///////////////////////////////////////////////////////////////////////////////
void LoomModule::print_config()
{
	print_module_label();
	LPrintln("Config:");
	LPrintln('\t', "Module Active       : ", (active)      ? "Enabled" : "Disabled" );
	LPrintln('\t', "Print Debug         : ", (print_debug) ? "Enabled" : "Disabled" );
	LPrintln('\t', "Print Verbosity     : ", enum_verbosity_string(print_verbosity) );
	LPrintln('\t', "Package Verbosity   : ", enum_verbosity_string(package_verbosity) );
}

///////////////////////////////////////////////////////////////////////////////
void LoomModule::print_state() 
{
	print_module_label();
	LPrintln("State:");
}

///////////////////////////////////////////////////////////////////////////////
void LoomModule::get_module_name(char* buf) 
{ 
	module_name_base.toCharArray(buf, 20);
}

///////////////////////////////////////////////////////////////////////////////
const char* LoomModule::get_module_name() 
{ 
	return module_name; 
}

///////////////////////////////////////////////////////////////////////////////
void LoomModule::set_print_verbosity(Verbosity v) 
{ 
	print_verbosity = v; 
	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Set print verbosity to: ", enum_verbosity_string(v));
	}
}

///////////////////////////////////////////////////////////////////////////////
Verbosity LoomModule::get_print_verbosity() 
{ 
	return print_verbosity; 
}

///////////////////////////////////////////////////////////////////////////////
void LoomModule::set_package_verbosity(Verbosity v) 
{ 
	package_verbosity = v;
	if (print_verbosity == Verbosity::V_HIGH) { 
		print_module_label();
		LPrintln("Set package verbosity verbosity to: ", enum_verbosity_string(v));
	}
}

///////////////////////////////////////////////////////////////////////////////
Verbosity LoomModule::get_package_verbosity() 
{ 
	return package_verbosity; 
}

///////////////////////////////////////////////////////////////////////////////
void LoomModule::set_active(bool enable) 
{ 
	active = enable; 
}

///////////////////////////////////////////////////////////////////////////////
bool LoomModule::get_active() 
{ 
	return active; 
}

///////////////////////////////////////////////////////////////////////////////
void LoomModule::set_print_debug(bool enable) 
{ 
	print_debug = enable; 
}

///////////////////////////////////////////////////////////////////////////////
bool LoomModule::get_print_debug() 
{ 
	return print_debug; 
}

///////////////////////////////////////////////////////////////////////////////
char* LoomModule::enum_verbosity_string(Verbosity v)
{
	switch(v) {
		case Verbosity::V_OFF  : return "Off";
		case Verbosity::V_LOW  : return "Low";
		case Verbosity::V_HIGH : return "High";
	}
}


// ///////////////////////////////////////////////////////////////////////////////
// // Maybe?
// //  - not pure because not everything will have a config (should default to empty function here)
// // Save a FlashStorage struct
// void LoomModule::save_config() {}

// ///////////////////////////////////////////////////////////////////////////////
// // Load a FlashStorage struct, true if valid
// bool LoomModule::load_config() {}

// ///////////////////////////////////////////////////////////////////////////////
// // Display config struct contents (as flash can be disabled this is a different method)
// void LoomModule::print_config_struct() {}




