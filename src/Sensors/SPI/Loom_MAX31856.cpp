
#include "Loom_MAX31856.h"


///////////////////////////////////////////////////////////////////////////////
Loom_MAX31856::Loom_MAX31856(	
		const char*		module_name,
		uint8_t			num_samples, 
		
		uint8_t			CS_pin,

		uint8_t			SPI_a,
		uint8_t			SPI_b,
		uint8_t			SPI_c,
		uint8_t			SPI_d
	) : LoomSPISensor( module_name, num_samples ) 
{
	this->module_type = ModuleType::MAX31856;

	// Hardware Serial
	// inst_max = new Adafruit_MAX31856(CS_pin);

	// Software Serial
	inst_max = new Adafruit_MAX31856(SPI_a, SPI_b, SPI_c, SPI_d);
	

	inst_max->begin();

	inst_max->setThermocoupleType(MAX31856_TCTYPE_K);
}

///////////////////////////////////////////////////////////////////////////////
Loom_MAX31856::Loom_MAX31856(JsonArrayConst p)
	: Loom_MAX31856( EXPAND_ARRAY(p, 7) ) {}

///////////////////////////////////////////////////////////////////////////////
Loom_MAX31856::~Loom_MAX31856() 
{
	delete inst_max;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31856::print_config()
{
	LoomSPISensor::print_config();
	
	LPrint("Thermocouple type: ");
	switch ( inst_max->getThermocoupleType() ) {
		case MAX31856_TCTYPE_B  : LPrintln("B Type"); break;
		case MAX31856_TCTYPE_E  : LPrintln("E Type"); break;
		case MAX31856_TCTYPE_J  : LPrintln("J Type"); break;
		case MAX31856_TCTYPE_K  : LPrintln("K Type"); break;
		case MAX31856_TCTYPE_N  : LPrintln("N Type"); break;
		case MAX31856_TCTYPE_R  : LPrintln("R Type"); break;
		case MAX31856_TCTYPE_S  : LPrintln("S Type"); break;
		case MAX31856_TCTYPE_T  : LPrintln("T Type"); break;
		case MAX31856_VMODE_G8  : LPrintln("Voltage x8 Gain mode"); break;
		case MAX31856_VMODE_G32 : LPrintln("Voltage x8 Gain mode"); break;
		default: LPrintln("Unknown"); break;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31856::print_measurements() 
{
	print_module_label();
	LPrintln("\t", "Temperature: ", temperature, " C");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31856::measure() 
{
	cj_temp = inst_max->readCJTemperature();
	temperature = inst_max->readThermocoupleTemperature();

	if (print_verbosity == Verbosity::V_HIGH) {
		// Check and print any faults
		uint8_t fault = inst_max->readFault();
		if (fault) {
			if (fault & MAX31856_FAULT_CJRANGE) LPrintln("Cold Junction Range Fault");
			if (fault & MAX31856_FAULT_TCRANGE) LPrintln("Thermocouple Range Fault");
			if (fault & MAX31856_FAULT_CJHIGH)  LPrintln("Cold Junction High Fault");
			if (fault & MAX31856_FAULT_CJLOW)   LPrintln("Cold Junction Low Fault");
			if (fault & MAX31856_FAULT_TCHIGH)  LPrintln("Thermocouple High Fault");
			if (fault & MAX31856_FAULT_TCLOW)   LPrintln("Thermocouple Low Fault");
			if (fault & MAX31856_FAULT_OVUV)    LPrintln("Over/Under Voltage Fault");
			if (fault & MAX31856_FAULT_OPEN)    LPrintln("Thermocouple Open Fault");
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31856::package(JsonObject json)
{
	package_json(json, module_name, 
		"Temp", temperature
	);
}

///////////////////////////////////////////////////////////////////////////////

