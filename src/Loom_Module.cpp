
#include "Loom_Module.h"

/////////////////////////////////////////////////////////////////////
char* LoomModule::enum_verbosity_string(Verbosity v)
{
	switch(v) {
		case Verbosity::V_OFF  : return "Off";
		case Verbosity::V_LOW  : return "Low";
		case Verbosity::V_HIGH : return "High";
	}
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
LoomModule::LoomModule() 
{
	snprintf(this->module_name, 20, "%s", "Unknown"); 

	active 				= true;
	print_debug 		= true;
	print_verbosity 	= Verbosity::V_LOW;
	package_verbosity 	= Verbosity::V_LOW;
	
	this->device_manager = nullptr;
}

/////////////////////////////////////////////////////////////////////
LoomModule::LoomModule( const char* module_name ) : LoomModule()
{

	snprintf(this->module_name, 20, "%s", module_name); 
}

/////////////////////////////////////////////////////////////////////
ModuleType LoomModule::get_module_type()
{
	return module_type;
}

/////////////////////////////////////////////////////////////////////

// --- PUBLIC METHODS ---
LoomManager* LoomModule::get_device_manager()
{
	return device_manager;
}

/////////////////////////////////////////////////////////////////////
void LoomModule::link_device_manager(LoomManager* LM)
{
	if (LM == nullptr) return;
	device_manager = LM;
}

/////////////////////////////////////////////////////////////////////
void LoomModule::second_stage_ctor()
{
	// do nothing unless overriden
}

/////////////////////////////////////////////////////////////////////
void LoomModule::print_module_label()
{
	LPrint("[", module_name, "] ");
}

/////////////////////////////////////////////////////////////////////
// Display configuration settings
void LoomModule::print_config()
{
	print_module_label();
	LPrintln("Config:");
	LPrintln('\t', "Module Active       : ", (active)      ? "Enabled" : "Disabled" );
	LPrintln('\t', "Print Debug         : ", (print_debug) ? "Enabled" : "Disabled" );
	LPrintln('\t', "Print Verbosity     : ", enum_verbosity_string(print_verbosity) );
	LPrintln('\t', "Package Verbosity   : ", enum_verbosity_string(package_verbosity) );
}

/////////////////////////////////////////////////////////////////////
// Display current state
void LoomModule::print_state() 
{

}

/////////////////////////////////////////////////////////////////////
// Copy module name into buffer
void LoomModule::get_module_name(char* buf) 
{ 
	strcpy(buf, module_name); 
}

/////////////////////////////////////////////////////////////////////
const char* LoomModule::get_module_name() 
{ 
	return module_name; 
}

/////////////////////////////////////////////////////////////////////
void LoomModule::set_print_verbosity(Verbosity v) 
{ 
	print_verbosity = v; 
}

/////////////////////////////////////////////////////////////////////
Verbosity LoomModule::get_print_verbosity() 
{ 
	return print_verbosity; 
}

/////////////////////////////////////////////////////////////////////
void LoomModule::set_package_verbosity(Verbosity v) 
{ 
	package_verbosity = v; 
}

/////////////////////////////////////////////////////////////////////
Verbosity LoomModule::get_package_verbosity() 
{ 
	return package_verbosity; 
}

/////////////////////////////////////////////////////////////////////
void LoomModule::set_active(bool enable) 
{ 
	active = enable; 
}

/////////////////////////////////////////////////////////////////////
bool LoomModule::get_active() 
{ 
	return active; 
}

/////////////////////////////////////////////////////////////////////
void LoomModule::set_print_debug(bool enable) 
{ 
	print_debug = enable; 
}

/////////////////////////////////////////////////////////////////////
bool LoomModule::get_print_debug() 
{ 
	return print_debug; 
}

/////////////////////////////////////////////////////////////////////
// Maybe?
//  - not pure because not everything will have a config (should default to empty function here)
// Save a FlashStorage struct
void LoomModule::save_config() {}

/////////////////////////////////////////////////////////////////////
// Load a FlashStorage struct, true if valid
bool LoomModule::load_config() {}

/////////////////////////////////////////////////////////////////////
// Display config struct contents (as flash can be disabled this is a different method)
void LoomModule::print_config_struct() {}


/////////////////////////////////////////////////////////////////////
// Optionally implementable interactive loop
void LoomModule::REPL_loop() {}

