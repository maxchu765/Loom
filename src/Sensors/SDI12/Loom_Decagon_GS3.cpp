
#include "Loom_Decagon_GS3.h"


///////////////////////////////////////////////////////////////////////////////
Loom_DecagonGS3::Loom_DecagonGS3(	
		const char*		module_name,
		uint8_t			num_samples 
	) 
	: LoomSDI12Sensor( module_name, num_samples ) 
{
	this->module_type = ModuleType::DecagonGS3;
}

///////////////////////////////////////////////////////////////////////////////
Loom_DecagonGS3::Loom_DecagonGS3(JsonArrayConst p)
	: Loom_DecagonGS3( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_DecagonGS3::print_config()
{
	LoomSDI12Sensor::print_config();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_DecagonGS3::print_measurements()
{

}

///////////////////////////////////////////////////////////////////////////////
void Loom_DecagonGS3::measure()
{

}

///////////////////////////////////////////////////////////////////////////////
void Loom_DecagonGS3::package(JsonObject json)
{
	// package_json(json, module_name, "Temp", temperature);
}

///////////////////////////////////////////////////////////////////////////////


