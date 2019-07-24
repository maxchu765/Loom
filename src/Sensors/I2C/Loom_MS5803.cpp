
#include "Loom_MS5803.h"


///////////////////////////////////////////////////////////////////////////////
Loom_MS5803::Loom_MS5803(
		byte			i2c_address, 
		int				mux_port,
		const char*		module_name
	)
	: LoomI2CSensor( module_name, i2c_address, mux_port )
{
	this->module_type = LoomModule::Type::MS5803;

	inst_MS5803 = new MS_5803(i2c_address, 512);
	bool setup = inst_MS5803->initializeMS_5803();

	if (!setup) active = false;

	print_module_label();
	LPrintln("\tInitialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_MS5803::Loom_MS5803(JsonArrayConst p)
	: Loom_MS5803( EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
Loom_MS5803::~Loom_MS5803() 
{
	delete inst_MS5803;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MS5803::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Pressure    : ", pressure , " mbar");
	LPrintln("\t", "Temperature : ", temp     , " C");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MS5803::measure()
{
	inst_MS5803->readSensor();

	pressure = inst_MS5803->pressure();
	temp     = inst_MS5803->temperature();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MS5803::package(JsonObject json)
{
	package_json(json, module_name, 
		"Pressure",	pressure,
		"Temp",		temp
	);
}

///////////////////////////////////////////////////////////////////////////////

