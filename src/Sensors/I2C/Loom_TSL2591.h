///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_TSL2591.h
/// @brief		File for the Loom_TSL2591 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Loom_I2C_Sensor.h"

#include <Adafruit_TSL2591.h>


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// TSL2591 Lux sensor module
// ###
class Loom_TSL2591 : public LoomI2CSensor
{

protected:

	Adafruit_TSL2591	inst_tsl2591;		///< Underlying TSL2591 sensor manager instance

	uint16_t			vis;				///< Measured visable lux
	uint16_t			ir;					///< Measured infra-red lux
	uint16_t			full;				///< Measured full spectrum lux

	uint8_t				gain_level;			///< Sensor gain level setting to use
	uint8_t				timing_level;		///< Sensor integration time setting

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// TSL2591 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x29> | {0x29} | I2C address
	/// \param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	/// \param[in]	gain_level				Set(Int) | <1> | { 1 } | Gain level
	/// \param[in]	timing_level			Set(Int) | <0> | { 0 } | Timing level
	Loom_TSL2591(
			byte			i2c_address			= 0x29,
			uint8_t			mux_port			= 255,
			uint8_t			gain_level			= 1,
			uint8_t			timing_level		= 0
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_TSL2591(JsonArrayConst p);

	/// Destructor
	~Loom_TSL2591() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_measurements() override;

private:

};


