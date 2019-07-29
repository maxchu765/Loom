#pragma once

#include "Loom_InternetPlat.h"
#include "EthernetLarge.h"
#include "SSLClient.h"


///////////////////////////////////////////////////////////////////////////////

// ### (LoomModule) | dependencies: [] | conflicts: []
/// InternetPlat built off of SSLClient running over an Ethernet Featherwing. Requires 7KB of free SRAM at runtime to use.
// ###
class Loom_Ethernet_I : public LoomInternetPlat
{

protected:
	
	SSLClient<EthernetClient> m_client;		/// Underlying Ethernet SSLclient instance
	EthernetUDP 	m_UDP;					/// Underlying Ethernet UDP instance
	
	byte 			m_mac[6];				/// The Ethernet MAC address
	IPAddress 		m_ip;					/// The devices IP address
	
	bool m_is_connected;					/// Whether or not ethernet initialized successfully
	
public:
	
//==============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	Loom_Ethernet_I(	
			const char* 			module_name		= "Ethernet",
			const JsonArrayConst	mac				= JsonArray(),
			const JsonArrayConst 	ip				= JsonArray()
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Ethernet_I( JsonArrayConst p );

	/// Destructor
	virtual ~Loom_Ethernet_I() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	// remember to close the socket!
	Client& connect_to_domain(const char* domain) override;

	/// Connect to internet
	void connect() override;

	/// Whether or not connected to internet
	/// \return True if connect, false otherwise
	bool is_connected() override;

	/// Get UTC time
	/// \return Time if found, 0 otherwise
	uint32_t get_time() override { return get_ntp_time(m_UDP); }

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void print_config() override;
	void print_state() override;

};

