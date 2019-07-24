
#include "Loom_MB1232.h"

#define RangeCommand    		0x51	//The Sensor ranging command has a value of 0x51
#define ChangeAddressCommand1 	0xAA	//These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand2 	0xA5


///////////////////////////////////////////////////////////////////////////////
Loom_MB1232::Loom_MB1232(
		byte			i2c_address,
		int				mux_port,
		const char*		module_name
	)
	: LoomI2CSensor( module_name, i2c_address, mux_port )
{	
	this->module_type = LoomModule::Type::MB1232;

	Wire.beginTransmission(i2c_address);

	Wire.write(RangeCommand);
	Wire.endTransmission();
	delay(100);
	
	Wire.requestFrom(i2c_address, byte(2));
	bool setup = (Wire.available() >= 2);

	if (!setup) active = false;

	print_module_label();
	LPrintln("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_MB1232::Loom_MB1232(JsonArrayConst p)
	: Loom_MB1232( EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MB1232::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Range: ", range, " cm");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MB1232::measure()
{
	Wire.beginTransmission(i2c_address);

	Wire.write(RangeCommand);
	Wire.endTransmission();
	delay(100);
	
	Wire.requestFrom(0x70, byte(2));
	if (Wire.available() >= 2) {
		// The sensor communicates two bytes, each a range. The
		// high byte is typically zero, in which case the low
		// byte is equal to the range, so only the range is transmitted.
		// The low byte will not be less than 20.
		byte high = Wire.read();
		byte low  = Wire.read();
		byte tmp  = Wire.read();
		
		range = (high * 256) + low;
	} else {
		print_module_label();
		LPrintln("Error reading from mb1232 (range)");
	}  
}


///////////////////////////////////////////////////////////////////////////////
void Loom_MB1232::package(JsonObject json)
{
	package_json(json, module_name, 
		"Range", range
	);
}

///////////////////////////////////////////////////////////////////////////////


