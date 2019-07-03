#pragma once

#include "Loom_CommPlat.h"

#include <RF24Network.h>
#include <RF24.h>


// // these will be defined in the config file
#define NRF_SELF_ADDRESS   01
#define NRF_FRIEND_ADDRESS 00

/// Max nRF message size
#define NRF_MESSAGE_SIZE 120


///////////////////////////////////////////////////////////////////////////////


// ### (LoomCommPlat) | dependencies: [] | conflicts: []
/// Nordic Radio communication platform module
// ###
class Loom_nRF : public LoomCommPlat
{

protected:

	RF24*				radio;				/// Underlying nRF24L01(+) radio object
	RF24Network*		network;			/// Network layer manager for radio

	uint8_t				address;			/// Device Address    (should this be part of LoomCommPlat? – maybe not as each platform handles addresses differently)
	uint8_t				friend_address;		/// Default address to send to

	uint8_t				data_rate;			/// Data rate
	uint8_t				power_level;		/// Power level to send at
	uint8_t				retry_count;		/// Number of transmission retries allowed
	uint16_t			retry_timeout;		/// Delay between transmission retries (in milliseconds)

	uint8_t				multicast_level;	/// Which level(s) of the network to broadcast to

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// nRF module constructor
	///
	/// \param[in]	module_name			String | <"nRF"> | null | nRF module name
	/// \param[in]	max_message_len		Set(Int) | <120> | {120("Max length")} | The maximum possible message length
	/// \param[in]	address 			Int | <01> | [0-99] | This device's nRF address
	/// \param[in]	friend_address 		Int | <00> | [0-99] | Default nRF address to send to
	/// \param[in]	data_rate			Set(Int) | <0> | {0("Default"), 1("250KBPS"), 2("1MBPS"), 3("2MBPS")} | Transmission data rate
	/// \param[in]	power_level			Set(Int) | <0> | {0("Default"), 1("Min"), 2("Low"), 3("High"), 4("Max")} | Transmission ower level
	/// \param[in]	retry_count 		Int | <3> | [0-15] | Max number of transmission retries
	/// \param[in]	retry_timeout 		Int | <200> | [20-500] | Delay between retransmissions (ms)
	/// \param[in]	multicast_level		Int | <1> | [1-3] | How many levels to propogate message through heirarchy
	Loom_nRF(
			const char*		module_name			= "nRF",
			uint16_t		max_message_len		= 120,
			uint8_t			address 			= NRF_SELF_ADDRESS,
			uint8_t			friend_address 		= NRF_FRIEND_ADDRESS,
			uint8_t			data_rate			= 1,
			uint8_t			power_level			= 0,
			uint8_t			retry_count 		= 3,
			uint16_t		retry_timeout 		= 200,
			uint8_t			multicast_level		= 1
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_nRF(JsonVariant p);

	/// Destructor
	~Loom_nRF();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool		receive(JsonObject json) override {}
	bool		send(JsonObject json, uint16_t destination) override {}
	bool		send(JsonObject json) override {}
	void		broadcast(JsonObject json) override {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	uint		get_address() override;

	/// Get address of default destination
	/// \return	Address of default destination device
	uint		get_friend_address();

	/// Get multicast(broadcast) level.
	/// nRF has varying degrees of broadcast corresponding to 
	/// depth to broadcast through network tree
	/// \return Multicast level
	uint8_t		get_multicast_level();

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	void		set_address(uint addr) override;

	/// Set address of default destination
	/// \param[in] addr		Address of destination device
	void		set_friend_address(uint addr);

	/// Set multicast(broadcast) level.
	/// nRF has varying degrees of broadcast corresponding to 
	/// depth to broadcast through network tree
	/// \param[in]	level 	Multicast level to set to
	void		set_multicast_level(uint8_t level);


//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

private:

};


