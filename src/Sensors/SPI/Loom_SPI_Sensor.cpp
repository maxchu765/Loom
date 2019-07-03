
#include "Loom_SPI_Sensor.h"


///////////////////////////////////////////////////////////////////////////////
LoomSPISensor::LoomSPISensor(	
		const char*		module_name,
		uint8_t			num_samples 
	) 
	: LoomSensor( module_name, num_samples ) {}

///////////////////////////////////////////////////////////////////////////////
void LoomSPISensor::print_config()
{
	LoomSensor::print_config();
}

///////////////////////////////////////////////////////////////////////////////

