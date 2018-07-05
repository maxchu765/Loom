// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
String data[NUM_FIELDS];


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_pushingbox();
// bool setup_ethernet();,
// void sendToPushingBox(int num_fields, char *server_name, char *devid);
void sendToPushingBox(OSCMessage &msg);
void sendToPushingBox(OSCBundle *bndl);

// ================================================================
// ===                          SETUP                           ===
// ================================================================

// ---  SETUP LORA ---
//
// Changes the state of the device to allow it to use LoRa. 
//
// @param rf95     An instance of the radio to be initialized
// @param manager  An instance of the manager to be initialized
//
void setup_pushingbox() 
{
	LOOM_DEBUG_Println("Setting up ethernet");
	#if is_ethernet == 1
		if(!setup_ethernet()) {
			LOOM_DEBUG_Println("Failed to setup ethernet");
		}
	#endif
}


// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================



// --- SEND TO PUSHINGBOX ---
// 
// Sends a get request to PushingBox
// Expects arguments of OSC message to be formated as:
// key, value, key, value...
// As the message router forwards messages not bundles, this is called by msg_router
// If you need to send a bundle, simply call the function (which is overloaded)
// with a correctly formatted OSCBundle
//
//
// @param msg  The message containing the information to send to PB.
//
void sendToPushingBox(OSCMessage &msg) 
{
	LOOM_DEBUG_Println("Sending to PushingBox");

	client.stop();
	if (client.connect(server_name, 80)) {  
		LOOM_DEBUG_Println("Connection good");
		client.print("GET /pushingbox?devid="); client.print(device_id); 
		LOOM_DEBUG_Print(".");
		for (int i = 0; i < msg.size(); i++) {
			if ((i % 2) == 0)
				client.print("&key" + String(i/2) + "=");
			else
				client.print("&val" + String(i/2) + "=");
			client.print(get_data_value(&msg, i));
			LOOM_DEBUG_Print(".");
		}
		LOOM_DEBUG_Println(".");
		client.println(" HTTP/1.1");
		client.print("Host: "); client.println(server_name);
		client.println("User-Agent: Arduino");
		client.println();

		LOOM_DEBUG_Println("Data done sending");	 
	} else {
		LOOM_DEBUG_Println("No Connection");
		#if is_ethernet == 1
			LOOM_DEBUG_Println("Failed to connect to PB, attempting to re-setup ethernet.");

			if (setup_ethernet()) {
				LOOM_DEBUG_Println("Successfully re-setup ethernet.");
			}
			#if LOOM_DEBUG == 1 
			else {
				Serial.println("Failed to re-setup ethernet.");
			}
			#endif
		#endif
	}
}


// --- SEND TO PUSHINGBOX ---
// 
// Sends a get request to PushingBox
// Expects OSC bundle to only have 1 message and its arguments to be formated as:
// key, value, key, value...
// Simple a wrapper for the version of this function that takes a message
// As the message router forwards messages not bundles
//
// @param msg  The message containing the information to send to PB.
//
void sendToPushingBox(OSCBundle *bndl) 
{
	sendToPushingBox(*(bndl->getOSCMessage(0)));
}





