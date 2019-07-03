#pragma once

#include "Loom_Module.h"


/// Different communication platforms available
enum class CommPlatform {
	WIFI, 		///< WiFi
	LORA, 		///< LoRa
	NRF,		///< Nordic Radio
	BLE,		///< Bluetooth
	SSER 		///< Slip Serial
};


///////////////////////////////////////////////////////////////////////////////


/// Abstract base of communication platform modules
class LoomCommPlat : public LoomModule
{

protected:

	uint16_t	max_message_len;			/// The maximum message length
	int16_t		signal_strength;			/// Rssi for Lora (need to determine what the other platforms use)

	StaticJsonDocument<2000> messageJson;	/// Document to read incoming data into

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomCommPlat(
			const char*		module_name,
			uint			max_message_len
		);

	/// Destructor
	virtual ~LoomCommPlat() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	virtual bool	cmd_route(JsonObject) override {}
	virtual void 	package(JsonObject json) override {};


	// Subclasses don't have to implement the following

	/// Build json from packet if any exists
	/// \param[out]	json	Json object to fill with incoming data
	virtual bool	receive(JsonObject json) {}

	/// Send json to a specific address
	/// \param[in]	json			Json package to send
	/// \param[in]	destination		Device to send to
	virtual bool	send(JsonObject json, uint16_t destination) {}

	/// Send json to default address.
	/// Calls send(JsonObject, uint16_t) with 
	virtual bool	send(JsonObject json) {}

	/// Broadcast data to all that can receive
	/// \param[in]	json	Json object to send
	virtual void	broadcast(JsonObject json) {};


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	virtual uint	get_address() = 0;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	virtual void	set_address(uint a) = 0;

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Get string of name associated with communication platform enum
	/// \param[in]	c 	CommPlatform to get c-string of
	/// \return c-string of communication platform
	static char*		enum_comm_plat_string(CommPlatform c);
	
	/// Get enum val associated with string representing communication platform
	/// \param[in]	s	C-string to get CommPlatform enum of
	/// \return CommPlatform 
	static CommPlatform string_to_enum_comm_plat(const char* s);

};


