
#ifndef LOOM_INTERRUPT_MANAGER_h
#define LOOM_INTERRUPT_MANAGER_h

#include "Loom_Module.h"


#include <OPEnS_RTC.h>
#include <AsyncDelay.h>


class LoomRTC; // Specify that LoomRTC exists, defined in own file



// Used to make function signatures easier to read 
// when returning function pointers
/// Typedef to for ISR function pointer readability
typedef void (*ISRFuncPtr)(); 




// enum IntType { INT_LOW, INT_HIGH, INT_CHANGE, INT_FALLING, INT_RISING };


struct IntDetails {
	ISRFuncPtr	ISR;			///< Function pointer to ISR. Set null if no interrupt linked
	byte		type;			///< Interrupt signal type to detect. LOW: 0, HIGH: 1, CHANGE: 2, FALLING: 3, INT_RISING: 4
	bool		is_immediate;	///< True if ISR is called directly upon interrupt, false if called next check of flags
	bool		enabled;		///< Whether or not this interrupt is enabled
};

struct TimerDetails {
	ISRFuncPtr	ISR;			///< Not a real ISR, just a function called if timer has expired
	uint		duration;		///< The timer duration
	bool		repeat;			///< Whether or not timer should repeat
	bool		enabled;		///< Whether or not this timer is enabled
};

struct StopWatchDetails {
	unsigned long	start_time;		///< The millis time when stopwatch started
	bool			enabled;		///< Whether or not this stopwatch is enabled
};


#define InteruptRange 16


#define MaxTimerCount 4
#define MaxStopWatchCount 2



class Loom_Interrupt_Manager : public LoomModule
{

protected:

	/// Enable or disable all interrupts 	-- currently only disables bottom halves
	bool			interrupts_enabled;

	/// List of interrupts configurations
	IntDetails		int_settings[InteruptRange];

	// Flags set by interrupts, indicating ISR bottom 
	// half should be called if not Null
	static bool 	interrupt_triggered[InteruptRange];

	/// Pointer to an RTC object for managing timers / timed interrupts
	LoomRTC*		RTC_Inst;

	/// Last time an alarm went off
	DateTime		last_alarm_time;



	AsyncDelay		timers[MaxTimerCount];
	TimerDetails	timer_settings[MaxTimerCount];


	StopWatchDetails	stopwatch_settings[MaxStopWatchCount];


	// interrupt_triggered for timers, also support immediate and delayed
public:

	// --- CONSTRUCTOR ---
	Loom_Interrupt_Manager( 
			char*		module_name		= "Interrupt_Manager",

			LoomRTC*	RTC_Inst		= NULL
		);


	// --- DESTRUCTOR ---
	~Loom_Interrupt_Manager();

	// Inherited methods
	void		print_config();
	void		print_state();
	void		measure() {}
	void		package(OSCBundle& bndl, char* suffix="") {}
	bool		message_route(OSCMessage& msg, int address_offset) {}

	void 		link_device_manager(LoomManager* LM);


	void		run_pending_ISRs();



// === === Interrupt Functions === ===


	/// All interrupts enable/disable
	/// \param[in]	state	Enable state to apply to all interrupts
	void		set_interrupts_enabled(bool state);
	/// Get global interrupt enable state
	/// \return		Global interrupt enable state
	bool		get_interrupts_enabled();

	/// Per interrupt enable
	/// \param[in]	pin		Interrupt pin to change enable state of
	/// \param[in]	state	The enable state to set pin to 
	void		set_enable_interrupt(byte pin, bool state);
	/// Get pin interrupt enable state
	/// \return		The enable state
	bool		get_enable_interrupt(byte pin);


	/// Register an ISR to an interrupt pin and its configuration
	// \param[in]	pin			Which pin to connect the interrupt on
	// \param[in]	ISR			ISR	function (Null if no interrupt linked)
	// \param[in]	type		Low, High, Change, Falling, Rising
	// \param[in]	immediate	Whether the interrupt runs immediately, else sets flag to check and runs ISR when flag checked
	void		register_ISR(byte pin, ISRFuncPtr ISR, byte type, bool immediate);

	/// Restores pin to default ISR, disables interrupt
	/// \param[in]	pin		The pin to unregister ISRs for
	/// \param[in]	type	What signal to configure default ISR to (default LOW)
	void		unregister_ISR(byte pin, byte type=LOW);  

	/// Checks the flags set by default ISRs, calls pending bottom half ISRs
	void		run_ISR_bottom_halves();

	/// Detaches then reattacheds interrupt according to settings.
	/// used to clear pending interrupts
	/// \param[in]	pin 	Pin to reset interrupts for
	void		interrupt_reset(byte pin);   



// === === RTC Alarm Functions === ===

	/// Set the RTC module to use for timers
	/// \param[in]	RTC_Inst	Pointer to the RTC object
	void		set_RTC_module(LoomRTC* RTC_Inst);
	/// Return pointer to the currently linked RTC object
	/// \return		Current RTC object
	LoomRTC*	get_RTC_module();


// Shorten names, maybe combine, taking behavior (e.g. relative/absolute) as parameter


// maybe rename relative/exact to TimeSpan/DateTime?
	

	/// Set RTC alarm relative time from now
	/// \param[in]	duration	How long before the alarm should go off
	bool		RTC_alarm_relative(TimeSpan duration);	

	/// Set RTC alarm relative time from now
	/// \param[in]	days		Days into the future the alarm should be set
	/// \param[in]	hours		Hours into the future the alarm should be set
	/// \param[in]	minutes		Minutes into the future the alarm should be set
	/// \param[in]	seconds		Seconds into the future the alarm should be set
	bool		RTC_alarm_relative(uint days, uint hours, uint minutes, uint seconds);
	

// maybe remove these 2 (leave in Sleep manager)
	// bool set_RTC_alarm_for_time_from_last_alarm_time(TimeSpan duration);
	// bool set_RTC_alarm_for_time_from_last_alarm_time(uint days, uint hours, uint minutes, uint seconds);
	



	/// Set RTC alarm for a specific time.
	/// Increments to next day at given hour, min, sec if specified time is in past
	/// \param[in]	future_time		Time to set alarm for 
	bool		RTC_alarm_exact(DateTime future_time);

	/// Set RTC alarm for a specific time.
	/// Forwards to RTC_alarm_exact that takes DateTime object
	/// \param[in]	hour		Hour to set alarm for
	/// \param[in]	minute		Minute to set alarm for
	/// \param[in]	second		Second to set alarm for
	bool		RTC_alarm_exact(uint hour, uint minute, uint second);
	




// === === AsyncDelay Timer Functions === ===


	/// Check if timers have elapsed, if so run associated 'ISR'
	void		check_timers();

	/// Configure specified timer
	void		register_timer(uint timer_num, unsigned long duration, ISRFuncPtr ISR, bool repeat);

	/// Clear specified timer 
	void		clear_timer(uint timer_num);


// === === Stopwatch Functions === ===





private:


	// Default ISRs that set flags, detach interrupt to prevent multiple triggering (reattached after checking flag)
	static void default_ISR_0()  { interrupt_triggered[0]  = true; };
	static void default_ISR_1()  { interrupt_triggered[1]  = true; };
	static void default_ISR_2()  { interrupt_triggered[2]  = true; };
	static void default_ISR_3()  { interrupt_triggered[3]  = true; };
	static void default_ISR_4()  { interrupt_triggered[4]  = true; };
	static void default_ISR_5()  { interrupt_triggered[5]  = true; };
	static void default_ISR_6()  { interrupt_triggered[6]  = true; };
	static void default_ISR_7()  { interrupt_triggered[7]  = true; };
	static void default_ISR_8()  { interrupt_triggered[8]  = true; };
	static void default_ISR_9()  { interrupt_triggered[9]  = true; };
	static void default_ISR_10() { interrupt_triggered[10] = true; };
	static void default_ISR_11() { interrupt_triggered[11] = true; };
	static void default_ISR_12() { interrupt_triggered[12] = true; };
	static void default_ISR_13() { interrupt_triggered[13] = true; };
	static void default_ISR_14() { interrupt_triggered[14] = true; };
	static void default_ISR_15() { interrupt_triggered[15] = true; };

// detaching interrupt did not seem to work
	// static void default_ISR_0()  { detachInterrupt(digitalPinToInterrupt(0));  interrupt_triggered[0]  = true;   }


	/// Array of the default ISRs that set flags
	const static ISRFuncPtr default_ISRs[InteruptRange];

};


#endif // of #ifndef LOOM_INTERRUPT_MANAGER_h












