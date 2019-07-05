#pragma once

#include "Loom_Module.h"

#include <OPEnS_RTC.h>
#include <AsyncDelay.h>


// #include <initializer_list>


#define InteruptRange 16		/// Number of interrupts
#define MaxTimerCount 4			/// Maximum number of timers
#define MaxStopWatchCount 2		/// Maximum numbr of stopwatches



// Specify that LoomRTC exists, defined in own file
class LoomRTC;


// Used to make function signatures easier to read
// when returning function pointers
/// Typedef to for ISR function pointer readability
using ISRFuncPtr = void (*)();
// using ISRFuncPtr = void *();


enum class ISR_Type { IMMEDIATE, CHECK_FLAG };



/// Contains information defining an interrupt's configuration
struct IntDetails {
	ISRFuncPtr	ISR;			///< Function pointer to ISR. Set null if no interrupt linked
	byte		type;			///< Interrupt signal type to detect. LOW: 0, HIGH: 1, CHANGE: 2, FALLING: 3, INT_RISING: 4
	ISR_Type	run_type;		///< True if ISR is called directly upon interrupt, false if called next check of flags
	bool		enabled;		///< Whether or not this interrupt is enabled
};

/// Contains information defining a timer's configuration
struct TimerDetails {
	ISRFuncPtr	ISR;			///< Not a real ISR, just a function called if timer has expired
	uint		duration;		///< The timer duration
	bool		repeat;			///< Whether or not timer should repeat
	bool		enabled;		///< Whether or not this timer is enabled
};

/// Contains information defining a stopwatch's configuration
struct StopWatchDetails {
	unsigned long	start_time;		///< The millis time when stopwatch started
	bool			enabled;		///< Whether or not this stopwatch is enabled
};


///////////////////////////////////////////////////////////////////////////////


// ### (LoomModule) | dependencies: [] | conflicts: []
/// Submanager for managing interrupts, timers, and stopwatches
// ###
class Loom_Interrupt_Manager : public LoomModule
{

protected:

	IntDetails		int_settings[InteruptRange];		/// List of interrupts configurations
	
	// Flags set by interrupts, indicating ISR bottom
	// half should be called if not Null
	static bool 	interrupt_triggered[InteruptRange];

	bool			interrupts_enabled;			/// Enable or disable all interrupts 	-- currently only disables bottom halves
	
	LoomRTC*		RTC_Inst;					/// Pointer to an RTC object for managing timers / timed interrupts	
	Loom_Sleep_Manager* Sleep_Manager;			/// Pointer to a Sleep Manager object
	
	DateTime		last_alarm_time;			/// Last time an alarm went off


	// millis timers

	AsyncDelay		timers[MaxTimerCount];
	TimerDetails	timer_settings[MaxTimerCount];


	StopWatchDetails	stopwatch_settings[MaxStopWatchCount];


	// interrupt_triggered for timers, also support immediate and delayed
public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Interrupt Manager module constructor.
	///
	/// \param[in]	module_name		String | <"Interrupt-Manager"> | null | Interrupt Manager module name
	/// \param[in]	RTC_Inst		Set(Int) | <0> | {0("Null")} | OLED module name
	Loom_Interrupt_Manager(
			const char*		module_name		= "Interrupt_Manager",
			LoomRTC*		RTC_Inst		= nullptr
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Interrupt_Manager(JsonVariant p);

	/// Destructor
	~Loom_Interrupt_Manager() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void 		package(JsonObject json) override {}
	bool		cmd_route(JsonObject) override {}

//=============================================================================
///@name	EXTERNAL INTERRUPT METHODS
/*@{*/ //======================================================================

	/// Register an ISR to an interrupt pin and its configuration
	/// \param[in]	pin			Which pin to connect the interrupt on
	/// \param[in]	ISR			ISR	function (Null if no interrupt linked)
	/// \param[in]	signal_type	Low, High, Change, Falling, Rising
	/// \param[in]	run_type	Whether the interrupt runs immediately, else sets flag to check and runs ISR when flag checked
	void		register_ISR(byte pin, ISRFuncPtr ISR, byte signal_type, ISR_Type run_type);

	/// Run any waiting ISRs.
	/// Flag was set by a top half ISR
	void		run_pending_ISRs();

	/// Restores pin to default ISR, disables interrupt
	/// \param[in]	pin				The pin to unregister ISRs for
	/// \param[in]	signal_type		What signal to configure default ISR to (default LOW)
	void		unregister_ISR(byte pin, byte signal_type=LOW);

	/// Detaches then reattacheds interrupt according to settings.
	/// used to clear pending interrupts
	/// \param[in]	pin 	Pin to reset interrupts for ...
	void		interrupt_reset(byte pin);


//=============================================================================
///@name	RTC ALARM METHODS
/*@{*/ //======================================================================

	// Shorten names, maybe combine, taking behavior (e.g. relative/absolute) as parameter

	// sleep_for
	/// Set RTC alarm relative time from now
	/// \param[in]	duration	How long before the alarm should go off
	bool		RTC_alarm_duration(TimeSpan duration);

	/// Set RTC alarm relative time from now
	/// \param[in]	days		Days into the future the alarm should be set
	/// \param[in]	hours		Hours into the future the alarm should be set
	/// \param[in]	minutes		Minutes into the future the alarm should be set
	/// \param[in]	seconds		Seconds into the future the alarm should be set
	bool		RTC_alarm_duration(uint days, uint hours, uint minutes, uint seconds);

	// sleep_until
	/// Set RTC alarm for a specific time.
	/// Increments to next day at given hour, min, sec if specified time is in past
	/// \param[in]	future_time		Time to set alarm for
	bool		RTC_alarm_at(DateTime future_time);

	/// Set RTC alarm for a specific time.
	/// Forwards to RTC_alarm_at that takes DateTime object
	/// \param[in]	hour		Hour to set alarm for
	/// \param[in]	minute		Minute to set alarm for
	/// \param[in]	second		Second to set alarm for
	bool		RTC_alarm_at(uint hour, uint minute, uint second);

// // sleep_for
	/// Set RTC alarm relative time from last alarm time
	/// \param[in]	duration	How long before the alarm should go off
	bool		RTC_alarm_duration_from_last(TimeSpan duration);

	/// Set RTC alarm relative time from last alarm time
	/// \param[in]	days		Days into the future the alarm should be set
	/// \param[in]	hours		Hours into the future the alarm should be set
	/// \param[in]	minutes		Minutes into the future the alarm should be set
	/// \param[in]	seconds		Seconds into the future the alarm should be set
	bool		RTC_alarm_duration_from_last(uint days, uint hours, uint minutes, uint seconds);


//=============================================================================
///@name	ASYNCDELAY TIMER METHODS
/*@{*/ //======================================================================

	/// Check if timers have elapsed, if so run associated 'ISR'
	void		check_timers();

	/// Configure specified timer
	/// \param[in]	timer_num		Timer to set
	/// \param[in]	duration		How long timer should take (seconds)
	/// \param[in]	ISR				ISR to run after timer goes off
	/// \param[in]	repeat			Whether or not to be a repeating alarm
	void		register_timer(uint timer_num, unsigned long duration, ISRFuncPtr ISR, bool repeat);

	/// Clear specified timer 
	/// \param[in]	timer_num		Timer to clear
	void		clear_timer(uint timer_num);

//=============================================================================
///@name	STOPWATCH METHODS
/*@{*/ //======================================================================


//=============================================================================
///@name	INTERNAL TIMER METHODS
/*@{*/ //======================================================================

	// https://github.com/GabrielNotman/RTCCounter

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get global interrupt enable state
	/// \return		Global interrupt enable state
	bool		get_interrupts_enabled();

	/// Get pin interrupt enable state
	/// \param[in]	pin		Pin to the the enable state of
	/// \return		The enable state
	bool		get_enable_interrupt(byte pin);

	/// Return pointer to the currently linked RTC object
	/// \return		Current RTC object
	LoomRTC*	get_RTC_module();

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Link a device manager.
	/// Overrides default by getting RTC pointer from 
	/// device manager if possible
	/// \param[in]	LM		Manager to link 
	void 		link_device_manager(LoomManager* LM) override;

	/// Set pointer to sleep Manager
	/// \param[in]	SM		Pointer to sleep manager
	void 		link_sleep_manager(Loom_Sleep_Manager* SM);

	/// All interrupts enable/disable
	/// \param[in]	state	Enable state to apply to all interrupts
	void		set_interrupts_enabled(bool state);

	/// Per interrupt enable
	/// \param[in]	pin		Interrupt pin to change enable state of
	/// \param[in]	state	The enable state to set pin to
	void		set_enable_interrupt(byte pin, bool state);

	/// Set the RTC module to use for timers
	/// \param[in]	RTC_Inst	Pointer to the RTC object
	void		set_RTC_module(LoomRTC* RTC_Inst);

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Get c-string of name associated with interrupt type enum
	/// \param[in]	type	Interrupt type 
	/// \return C-string of interrupt type
	const static char* interrupt_type_to_string(int type);


private:

	/// Checks the flags set by default ISRs, calls pending bottom half ISRs
	void		run_ISR_bottom_halves();

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


