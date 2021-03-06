///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Decagon_5TM.cpp
/// @brief		File for Loom_Decagon5TM implementation. Incomplete.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Loom_Decagon_5TM.h"


///////////////////////////////////////////////////////////////////////////////
Loom_Decagon5TM::Loom_Decagon5TM(	
		const uint8_t		num_samples 
	) 
	: LoomSDI12Sensor( "5TM", Type::Decagon5TM, num_samples ) 
{

}

///////////////////////////////////////////////////////////////////////////////
Loom_Decagon5TM::Loom_Decagon5TM(JsonArrayConst p)
	: Loom_Decagon5TM( (uint8_t)p[0] ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Decagon5TM::print_config() const
{
	LoomSDI12Sensor::print_config();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Decagon5TM::print_measurements() const
{

}

///////////////////////////////////////////////////////////////////////////////
void Loom_Decagon5TM::measure()
{

}

///////////////////////////////////////////////////////////////////////////////
void Loom_Decagon5TM::package(JsonObject json)
{
	// package_json(json, module_name, "Temp", temperature);
}

///////////////////////////////////////////////////////////////////////////////
