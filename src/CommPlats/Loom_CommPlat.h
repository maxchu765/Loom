///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_CommPlat.h
/// @brief		File for LoomCommPlat definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Loom_Module.h"


///////////////////////////////////////////////////////////////////////////////
///
/// Abstract base of communication platform modules
///
/// All communication platform modules inherit from this class.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_comm_plat.html)
///
///////////////////////////////////////////////////////////////////////////////
class LoomCommPlat : public LoomModule
{

protected:

	uint16_t	max_message_len;	///< The maximum message length
	int16_t		signal_strength;	///< RSSI for Lora (need to determine what the other platforms use)

	/// Document to read incoming data into.
	/// CommPlatforms need their own JsonDocument because an incoming message
	/// can only be deserialized into JsonDocuments, not JsonObjects.
	/// And it seemed bad design to pass around references to the LoomManager's
	/// internal JsonDocument. 
	/// Especially as the LoomManager is intended to be non-mandatory for usage of Loom
	StaticJsonDocument<1500> messageJson;

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	module_name		Name of the module (provided by derived classes)
	/// @param[in]	module_type		Type of the module (provided by derived classes)
	/// @param[in]	max_message_len	The maximum possible message length
	LoomCommPlat(
			const char*				module_name,
			const LoomModule::Type	module_type,
			const uint16_t			max_message_len
		);

	/// Destructor
	virtual ~LoomCommPlat() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	virtual void 	package(JsonObject json) override {};

	/// Build json from packet if any exists
	/// @param[out]	json	Json object to fill with incoming data
	virtual bool	receive(JsonObject json) {}

	/// Version of send for use with LoomManager.
	/// Accesses Json from LoomManager
	/// @return True if packet received
	bool			receive();

	/// Receive, but block until packet received, or timeout reached
	/// @param[out]	json			Json object to fill with incoming data
	/// @param[out]	max_wait_time	Maximum number of milliseconds to block for
	/// @return True if packet received
	bool			receive_blocking(JsonObject json, uint max_wait_time);

	/// Version of receive_blocking for use with LoomManager.
	/// Accesses Json from LoomManager
	/// @param[out]	max_wait_time	Maximum number of milliseconds to block for
	/// @return True if packet received	
	bool			receive_blocking(const uint max_wait_time);

	/// Send json to a specific address
	/// @param[in]	json			Json package to send
	/// @param[in]	destination		Device to send to
	/// @return True if packet sent successfully
	virtual bool	send(JsonObject json, const uint8_t destination) {}

	/// Version of send for use with LoomManager.
	/// Accesses Json from LoomManager
	/// @param[in]	destination		Address of destination device
	/// @return True if packet sent successfully, false otherwise
	bool			send(const uint8_t destination);

	/// Broadcast data to all that can receive.
	/// Derived classes can optionally provide an implementation for this,
	/// As supported by the radio/platform's library
	/// @param[in]	json	Json object to send
	virtual void	broadcast(JsonObject json) {};

	/// Version of send for use with LoomManager.
	/// Accesses Json from LoomManager
	void			broadcast();

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the address of this device.
	/// Each platform may have a different addressing scheme
	/// @return The address of this device
	virtual uint8_t	get_address() const = 0;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set the address of this device.
	/// Each platform may have a different addressing scheme.
	/// Changing the device's address is not recommended.
	/// @param[in]	a 	The address to set this device to
	virtual void	set_address(const uint8_t a) = 0;

protected:

	/// Serialize a JsonObject into a MessagePack buffer.
	/// Also contains buffer memsetting and print statements
	/// @param[in]	json		JsonObject to serialize
	/// @param[out]	buffer		Buffer to fill with MessagePack of json
	/// @param[in]	max_len		Length of buffer
	/// return True if success
	bool	json_to_msgpack_buffer(JsonObjectConst json, char* buffer, const uint16_t max_len) const;

	/// Deserialize a MessagePack buffer into a JsonObject.
	/// Also clears the json, contains prints and error checks.
	/// @param[in]	buffer		Buffer to deserialize
	/// @param[out]	json		JsonObject to deserialize into
	/// @param[in]	max_len		Length of buffer
	/// return True if success
	bool	msgpack_buffer_to_json(const char* buffer, JsonObject json);

};







