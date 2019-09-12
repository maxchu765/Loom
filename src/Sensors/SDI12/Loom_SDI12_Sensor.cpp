///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SDI12_Sensor.cpp
/// @brief		File for LoomSDI12Sensor implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Loom_SDI12_Sensor.h"


///////////////////////////////////////////////////////////////////////////////
LoomSDI12Sensor::LoomSDI12Sensor(	
		const char*			module_name,
		LoomModule::Type	module_type,
		uint8_t				num_samples 
	) 
	: LoomSensor( module_name, module_type, num_samples ) {}

///////////////////////////////////////////////////////////////////////////////
