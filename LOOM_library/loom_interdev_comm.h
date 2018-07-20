

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void print_known_devices();
void broadcast_all_platforms(OSCBundle* bndl);
void send_to_known_devices(OSCBundle* bndl);
void device_search();
void respond_to_device_poll(OSCMessage &msg);
void update_known_devices(OSCMessage &msg);

// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================


// --- PRINT KNOWN DEVICES ---
//
// Prints the information about all of the devices 
// that this device knows and can communicate with
//
void print_known_devices()
{
	#if LOOM_DEBUG == 1
		LOOM_DEBUG_Println("Known Devices:");
		for (int i = 0; i < num_known_devices; i++) {
			LOOM_DEBUG_Println2("\tDevice ID: ",     known_devices[i].device_id);
			LOOM_DEBUG_Println2("\tInstance #: ",    known_devices[i].inst_num);
			LOOM_DEBUG_Println2("\tNum Platforms: ", known_devices[i].num_platforms);
			LOOM_DEBUG_Println("= = = = =");
			for (int j = 0; j < known_devices[i].num_platforms; j++) {
				switch (known_devices[i].platforms[j]) {
					case WIFI :
						LOOM_DEBUG_Println("\tWiFi");
						LOOM_DEBUG_Println2("\t\tPort: ", known_devices[i].udp_port);
						LOOM_DEBUG_Print("\t\tIP: ");
						LOOM_DEBUG_Print2( known_devices[i].ip_addr[0], ".");
						LOOM_DEBUG_Print2( known_devices[i].ip_addr[1], ".");
						LOOM_DEBUG_Print2( known_devices[i].ip_addr[2], ".");
						LOOM_DEBUG_Println(known_devices[i].ip_addr[3]);
						break;
					case LORA :
						LOOM_DEBUG_Println("LoRa");
						LOOM_DEBUG_Println2("\t\tDevice Address: ", known_devices[i].dev_address);
						break;
					case NRF :
						LOOM_DEBUG_Println("nRF");
						LOOM_DEBUG_Println2("\t\tDevice Address: ", known_devices[i].dev_address);
						break; 
					case ETHERNET :
						LOOM_DEBUG_Println("Ethernet");
						break;
				}	
			}
		}

	#endif
}



// --- BROADCAST ALL PLATFORMS ---
//
// Broadcast a message on all supported platforms
//
// @param bndl  The bundle to be broadcasted
//
void broadcast_all_platforms(OSCBundle* bndl)
{
	LOOM_DEBUG_Println("Broadcasting to on all platforms");

	#if is_wifi == 1
	wifi_send_bundle(bndl, 9440); // send on common port
	#endif

	#if is_lora == 1
// probably need to iterate over addresses or something if there isnt a broadcast function
	lora_send_bundle(bndl); 
	#endif

	#if is_nrf == 1
// probably need to iterate over addresses or something if there isnt a broadcast function
	nrf_send_bundle(bndl);
	#endif

	#if is_ethernet == 1
// still need to confirm ethernet inter-device communication first
	#endif

}



// --- SEND TO KNOWN DEVICES
//
// Send a message to all the devices in the known messages list
//
// @param bndl  The bundle to send to known devices
// 
void send_to_known_devices(OSCBundle* bndl)
{
	LOOM_DEBUG_Println("Sending to all known devices");
}



// --- DEVICE SEARCH ---
//
// Iterates over each enabled communication platforms,
// searching for devices by sending out a poll request
// Calls broadcast_all_platforms to do this 
//
void device_search() 
{
	LOOM_DEBUG_Println("Searching for devices");

	OSCBundle bndl; 
	bndl.empty();
	char addressString[255];
// sprintf(addressString, "%s%s", packet_header_string, "/DeviceChannelPoll");
	sprintf(addressString, "/%s%s", FAMILY, "/DeviceChannelPoll");


	bndl.add(addressString);

	// now call broadcast_all_platforms
	broadcast_all_platforms(&bndl);
}



// --- RESPONSE TO DEVICE POLL 
//
// Hub has requested all nodes respond presence with identifying info
// 
// @param msg  The poll message, routed from common_msg_router, requesting device response
//
		// maybe rename to better match 'device search'
void respond_to_device_poll(OSCMessage &msg) 
{
	LOOM_DEBUG_Println("Responding to device poll");

	OSCBundle bndl; 
	bndl.empty();
	char addressString[255];

	// sprintf(addressString, "%s%s", packet_header_string, "/DevicePollResponse");
	sprintf(addressString, "/%s%s", FAMILY, "/DevicePollResponse");

	char buf[20];
	sprintf(buf, "%s%d", DEVICE, configuration.instance_number);
	OSCMessage tmpMsg = new OSCMessage(addressString);
	tmpMsg.add(buf);
// Should the bundle be addressed to just FAMILY, or the device that send the request?

	// Add details about this device

	// Instance number
	tmpMsg.add((int32_t)configuration.instance_number);
	
	// Platforms and Addressing
	#if is_wifi == 1
		tmpMsg.add( (int32_t) WIFI );
		tmpMsg.add( (int32_t)configuration.config_wifi.localPort );
		for (int i=0; i<4; i++) 
			tmpMsg.add( (int32_t)configuration.config_wifi.ip[i] );
	#endif
	#if is_lora == 1
		tmpMsg.add( (int32_t) LORA );
		tmpMsg.add( (hub_node_type) ? LORA_NODE_ADDRESS : LORA_HUB_ADDRESS );
	#endif
	#if is_nrf == 1
		tmpMsg.add( (int32_t) NRF );
		tmpMsg.add( (hub_node_type) ? NRF_NODE_ADDRESS : NRF_HUB_ADDRESS );
	#endif
	#if is_ethernet == 1
		// tmpMsg.add( (int32_t) ETHERNET );
	#endif

	bndl.add(tmpMsg);

	// LOOM_DEBUG_Println("Response Bundle:");
	// print_bundle(&bndl);

// Respond on the platform that the poll was made on, should probably just be one of the msg arguments (can use the enum)

	wifi_send_bundle_common(&bndl);
	bndl.empty();             // Empty the bundle to free room for a new one
}



// --- UPDATE KNOWN DEVICES
//
// A node has responsed to device poll,
// Iterate over current known devices, checking device identifiers
// If the end is reached, add new entry
// If device is already in list, only update if there is info change from current
//
// @param msg  The poll response from a node device
//
void update_known_devices(OSCMessage &msg)
{
	LOOM_DEBUG_Println("Updating known devices");

	char buf[80], msgDevID[20];
	msg.getAddress(buf); // this gets the complete address


	// LOOM_DEBUG_Println("Message contents:");
	// LOOM_DEBUG_Println2("\t", buf);
	// for (int i = 0; i < msg.size(); i++) {
	// 	LOOM_DEBUG_Println2("\t", get_data_value(&msg, i));
	// }

	// This grabs the device type and instance number from the message address
	// snprintf(msgDevID, (strrchr(buf,'/') - strchr(buf+1,'/')), "%s", strchr(buf+1,'/')+1);
	sprintf(msgDevID, "%s", get_data_value(&msg, 0).c_str());
	bool found = false;
	for (int i = 0; i < num_known_devices; i++) {

		// If match with stored device identifier
		if ( strcmp(msgDevID, known_devices[i].device_id) == 0 ) {

			// dont do anything currently, assume rest of data is up to date, might change this in the future

			LOOM_DEBUG_Println3("Device: ", msgDevID, "already in list of known devices");
			found = true;
			break;
		}

	}

	// Handle if message was from is a new device
	// Add new entry to 'known_devices'
	if (!found) {
		LOOM_DEBUG_Println3("Device: ", msgDevID, " is new, adding to known device list");

		DeviceInfo new_entry;

		// Device Identifiers
		sprintf(new_entry.device_id, "%s", msgDevID);
		new_entry.inst_num  = msg.getInt(1);

		// Set to known invalid numbers as not all values are set
		new_entry.num_platforms = 0;
		new_entry.dev_address = 99;
		new_entry.udp_port    = 99;
		new_entry.ip_addr[0]  = 99;
		new_entry.ip_addr[1]  = 99;
		new_entry.ip_addr[2]  = 99;
		new_entry.ip_addr[3]  = 99;

		// Platforms and Addressing
		int i = 2, platIdx = 0; 
		while ( i < msg.size() ){
			CommPlatform plat = static_cast<CommPlatform>(msg.getInt(i++));
			new_entry.platforms[platIdx++] = plat;
			switch(plat) {
				case WIFI:
					// Get Port
					new_entry.udp_port = msg.getInt(i++);
					// Get IP Address
					for (int j=0; j<4; j++) 
						new_entry.ip_addr[j] = msg.getInt(i++);
					break;
				case LORA:
					new_entry.dev_address = msg.getInt(i++);
					break;
				case NRF:
					new_entry.dev_address = msg.getInt(i++);
					break;
				case ETHERNET:
					// In progress
					break;
			}
		}
		new_entry.num_platforms = platIdx;

		// Add to list
		known_devices[num_known_devices++] = new_entry;
	}
}
