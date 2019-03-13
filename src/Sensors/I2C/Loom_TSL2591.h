
#ifndef LOOM_TSL2591_h
#define LOOM_TSL2591_h

#include "Loom_I2C_Sensor.h"

#include <Adafruit_TSL2591.h>



class Loom_TSL2591 : public LoomI2CSensor
{

protected:

	/// Underlying TSL2591 sensor manager instance
	Adafruit_TSL2591	inst_tsl2591;

	/// Measured visable lux
	uint16_t			vis;
	/// Measured infra-red lux
	uint16_t			ir;
	/// Measured full spectrum lux
	uint16_t			full;

	/// Sensor gain level setting to use
	uint8_t				gain_level;
	/// Sensor integration time setting
	uint8_t				timing_level;

public:

	// --- CONSTRUCTOR ---
	Loom_TSL2591(	byte i2c_address 			= 0x29,
					
					char* module_name 			= "TSL2591",
					char* sensor_description 	= "Lux",

					uint8_t gain_level 			= 1, // document 
					uint8_t timing_level 		= 0  // document
				);


	// --- DESTRUCTOR ---
	virtual ~Loom_TSL2591();

	// Inherited (Overriding) Methods
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");	

private:

};


#endif
