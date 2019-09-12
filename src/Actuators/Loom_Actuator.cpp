///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Actuator.cpp
/// @brief		File for LoomActuator implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "Loom_Actuator.h"


///////////////////////////////////////////////////////////////////////////////
LoomActuator::LoomActuator(
		const char*			module_name,
		LoomModule::Type	module_type
	) : LoomModule( module_name, module_type ) {}

///////////////////////////////////////////////////////////////////////////////
