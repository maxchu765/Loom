#pragma once

#include "Loom_Module.h"
#include "./InternetPlats/Loom_InternetPlat.h"
#include "./RTC/Loom_RTC.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomModule) | dependencies: [] | conflicts: []
/// Glue code to synchronize an RTC using an InternetPlat. Always synchronizes the RTC from Loom_Interrupt_Manager::get_RTC_module().
// ###
class LoomNTPSync : public LoomModule
{

protected:

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// NTP Sync module constructor.
	///
	/// \param[in]  sync_interval_hours		Int | <0> | [0-999] | What hourly period to sync the RTC, zero for once on startup.
	LoomNTPSync(
		const uint			sync_interval_hours		= 0
	);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	LoomNTPSync(JsonArrayConst p);

	/// Destructor
	~LoomNTPSync() = default;

	/// Sync the RTC using NTP from the internet platform specified
	void	second_stage_ctor() override;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

    void Run() {}
	void		measure();
	void 		package(JsonObject json) override { /* do nothing */ };
	bool		dispatch(JsonObject) override { /* do nothing */}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;	

private:
	
	/// the actual synchronization function
	DateTime m_sync_rtc();
	
	/// enumerate errors
	enum class Error {
		OK,
		INVAL_DEVICE_MANAGE, ///< Failed to recieve a Loom Device Manager
		INVAL_INTERNET, ///< Failed to find the correct internet object
		INVAL_TIME,     ///< Retrieved a time that was invalid
		INVAL_RTC,      ///< Failed to find a RTC
		NON_START,      ///< No attempt was made to sync
		NO_CONNECTION, ///< Repeated attempts were made to sync, but there was no response
	};
	
	/// Store the Internet Plat from second stage contsruction
	// const uint m_internet_module_index;
	/// Store the sync interval, in hours
	const uint			m_sync_interval;
	/// Store the Internet Plat from second stage contsruction
	LoomInternetPlat*	m_internet;
	/// Store the RTC pointer so we can check the time
	LoomRTC*			m_rtc;
	/// Store when next to change the RTC
	DateTime			m_next_sync;
	/// Store if we've successfully accomplished our task
	Error				m_last_error;
};


