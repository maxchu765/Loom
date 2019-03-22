
#ifndef LOOM_MB1232_h
#define LOOM_MB1232_h


#include "Loom_I2C_Sensor.h"


#define RangeCommand    		0x51	//The Sensor ranging command has a value of 0x51
#define ChangeAddressCommand1 	0xAA	//These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand2 	0xA5


class Loom_MB1232 : public LoomI2CSensor
{

protected:

	/// Measure distance (cm)
	int range;

public:

	// --- CONSTRUCTOR ---
	Loom_MB1232(	
			byte		i2c_address				=  0x70, // 0x70   // MB1232_I2C_Address,
			
			char*		module_name				=  "MB1232",   // MB1232_Module_Name,
			char*		sensor_description		= "Sonar"    // MB1232_Sensor_Description 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_MB1232();

	// Inherited (Overriding) Methods
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");

private:

};


#endif
