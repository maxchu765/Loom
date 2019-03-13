
#ifndef LOOM_SERVO_h
#define LOOM_SERVO_h

#include "Loom_Actuator.h"

#include <Adafruit_PWMServoDriver.h>


#define NUM_SERVOS 8



class Loom_Servo : public LoomActuator
{

protected:

	/// Underlying servo driver instance
	Adafruit_PWMServoDriver servo_driver = Adafruit_PWMServoDriver();

	/// Number of servos
	uint8_t		servo_count;

	/// Positions of the servos
	int*		positions;

public:

	// --- CONSTRUCTOR ---
	Loom_Servo( char* module_name = "Servo",

				uint8_t servo_count = NUM_SERVOS

				);

	// --- DESTRUCTOR ---
	virtual ~Loom_Servo();

	// Inherited (Overriding) Methods
	void		print_config();
	void		print_state();
	void		package(OSCBundle& bndl, char* suffix="");
	bool		message_route(OSCMessage& msg, int address_offset);

	// Actuator Control
	void		set_degree(int servo, int degree);
	void		set_degree(OSCMessage& msg);

private:

};


#endif // of LOOM_SERVO_h
