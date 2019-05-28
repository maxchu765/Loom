
#include "Loom_Decagon_GS3.h"

/////////////////////////////////////////////////////////////////////
Loom_DecagonGS3::Loom_DecagonGS3(	
		const char*		module_name,
		uint8_t			num_samples 
	) 
	: LoomSDI12Sensor( module_name, num_samples ) 
{
	this->module_type = ModuleType::DecagonGS3;



}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_DecagonGS3::Loom_DecagonGS3(JsonVariant p)
	: Loom_DecagonGS3(p[0], p[1]) 
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_DecagonGS3::~Loom_DecagonGS3() 
{

}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void Loom_DecagonGS3::print_config()
{
	LoomSDI12Sensor::print_config();

}

/////////////////////////////////////////////////////////////////////
void Loom_DecagonGS3::print_measurements()
{

}

/////////////////////////////////////////////////////////////////////
void Loom_DecagonGS3::measure()
{

}

/////////////////////////////////////////////////////////////////////
void Loom_DecagonGS3::package(OSCBundle& bndl, char* suffix)
{

}

