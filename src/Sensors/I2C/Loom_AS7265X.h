#pragma once

#include "Loom_I2C_Sensor.h"

#include "SparkFun_AS7265X.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// AS72625X 3 Spectrum (Visible, near IR, UV) spectral sensor module
// ###
class Loom_AS7265X : public LoomI2CSensor
{

protected:

	AS7265X		inst_AS7265X;		///< Underlying AS7265X sensor manager instance

	uint16_t	uv[6];				///< Measured UV bands values
	uint16_t	color[6];			///< Measured color bands values
	uint16_t	nir[6];				///< Measured near-infra-red bands values

	bool		use_bulb;			///< Whether or not to use the bulb

	uint8_t		gain;				///< Gain setting
	uint8_t		mode;				///< Sensor mode
	uint8_t		integration_time;	///< Integration time setting

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// AS72625X module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x49> | {0x49} | I2C address
	/// \param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	/// \param[in]	use_bulb				Bool | <false> | {true, false} | Whether or not to use bulb
	/// \param[in]	gain					Set(Int) | <1> | { 0("1x"), 1("3.7x"), 2("16x"), 3("64x") } | Gain level
	/// \param[in]	mode					Set(Int) | <3> | { 0("4 channels out of 6"), 1("Different 4 channels out of 6"), 2("All 6 channels continuously"), 3("One-shot reading of all channels") } | Read mode
	/// \param[in]	integration_time		Int | <50> | [0-255] | Integration time (time will be 2.8ms * [integration value])
	Loom_AS7265X(
			byte			i2c_address			= 0x49,
			uint8_t			mux_port			= 255,
			bool			use_bulb			= false,
			uint8_t			gain				= 64,
			uint8_t			mode				= AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT,	// 4 channel, other 4 channel, 6 chan, or 6 chan one shot // AS7265X_Mode,
			uint8_t			integration_time	= 50
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_AS7265X(JsonArrayConst p);

	/// Destructor
	~Loom_AS7265X() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_measurements() override;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set whether not bulb is used for active light source
	/// \param[in]	enable	Whether or not to enable 
	void		enable_bulb(bool e) { use_bulb = e; }

	/// Set gain.
	/// 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
	/// \param[in]	gain	Gain level: 
	void		set_gain(uint8_t gain) { inst_AS7265X.setGain(gain); }

	/// Set mode.
	/// 4 channel, other 4 channel, 6 chan, or 6 chan one shot.
	/// \param[in]	mode	Mode
	void		set_mode(uint8_t mode) { inst_AS7265X.setMeasurementMode(mode); }

	/// Set integration time.
	/// 50 * 2.8ms = 140ms. 0 to 255 is valid.  (49 is default)
	/// If you use Mode 2 or 3 (all the colors) then integration time is double. 140*2 = 280ms between readings.
	void		set_integration_time(uint8_t time) { inst_AS7265X.setIntegrationCycles(time); }

private:

};


