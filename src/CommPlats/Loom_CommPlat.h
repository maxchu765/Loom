#pragma once

#include "Loom_Module.h"


///////////////////////////////////////////////////////////////////////////////


/// Abstract base of communication platform modules
class LoomCommPlat : public LoomModule
{

protected:

	uint16_t	max_message_len;			/// The maximum message length
	int16_t		signal_strength;			/// Rssi for Lora (need to determine what the other platforms use)

	// CommPlatforms need their own JsonDocument because an incoming message
	// can only be deserialized into JsonDocuments, not JsonObjects.
	// And it seemed bad design to pass around references to the LoomManager's
	// internal JsonDocument. 
	// Especially as the LoomManager is intended to be non-mandatory for usage of Loom
	StaticJsonDocument<1500> messageJson;	/// Document to read incoming data into

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomCommPlat(
			const char*			module_name,
			LoomModule::Type	module_type,
			uint16_t			max_message_len
		);

	/// Destructor
	virtual ~LoomCommPlat() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	virtual bool	dispatch(JsonObject) override {}
	virtual void 	package(JsonObject json) override {};

	/// Build json from packet if any exists
	/// \param[out]	json	Json object to fill with incoming data
	virtual bool	receive(JsonObject json) {}

	/// Version of send for use with LoomManager.
	/// Accesses Json from LoomManager
	/// \return True if packet received
	bool			receive();

	/// Receive, but block until packet received, or timeout reached
	/// \param[out]	json			Json object to fill with incoming data
	/// \param[out]	max_wait_time	Maximum number of milliseconds to block for
	/// \return True if packet received
	bool			receive_blocking(JsonObject json, uint max_wait_time);

	/// Version of receive_blocking for use with LoomManager.
	/// Accesses Json from LoomManager
	/// \param[out]	max_wait_time	Maximum number of milliseconds to block for
	/// \return True if packet received	
	bool			receive_blocking(uint max_wait_time);

	/// Send json to a specific address
	/// \param[in]	json			Json package to send
	/// \param[in]	destination		Device to send to
	/// \return True if packet sent successfully
	virtual bool	send(JsonObject json, uint8_t destination) {}

	/// Version of send for use with LoomManager.
	/// Accesses Json from LoomManager
	/// \param[in]	destination		Address of destination device
	/// \return True if packet sent successfully
	bool			send(uint8_t destination);

	/// Broadcast data to all that can receive
	/// \param[in]	json	Json object to send
	virtual void	broadcast(JsonObject json) {};

	/// Version of send for use with LoomManager.
	/// Accesses Json from LoomManager
	void			broadcast();

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	virtual uint8_t	get_address() = 0;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	virtual void	set_address(uint8_t a) = 0;

protected:

	/// Serialize a JsonObject into a MessagePack buffer.
	/// Also contains buffer memsetting and print statements
	/// \param[in]	json		JsonObject to serialize
	/// \param[out]	buffer		Buffer to fill with MessagePack of json
	/// \param[in]	max_len		Length of buffer
	/// return True if success
	bool	json_to_msgpack_buffer(JsonObjectConst json, char* buffer, uint16_t max_len);

	/// Deserialize a MessagePack buffer into a JsonObject.
	/// Also clears the json, contains prints and error checks.
	/// \param[in]	buffer		Buffer to deserialize
	/// \param[out]	json		JsonObject to deserialize into
	/// \param[in]	max_len		Length of buffer
	/// return True if success
	bool	msgpack_buffer_to_json(const char* buffer, JsonObject json);

};







