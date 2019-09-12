///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Max_Pub.cpp
/// @brief		File for Loom_MaxPub implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Loom_Max_Pub.h"
#include "../SubscribePlats/Loom_Max_Sub.h"
#include "../Loom_Manager.h"

///////////////////////////////////////////////////////////////////////////////
Loom_MaxPub::Loom_MaxPub(
		LoomModule::Type	internet_type
	)   
	: LoomPublishPlat( "MaxPub", Type::MaxPub, internet_type )
	, remoteIP({192,168,1,255})
{}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxPub::second_stage_ctor() 
{
	LoomPublishPlat::second_stage_ctor();

	UDP_port = UDP_SEND_OFFSET + ((device_manager) ? device_manager->get_instance_num() : 0);

	// Get new UDP pointer	
	if (m_internet != nullptr) {
		UDP_Inst = m_internet->open_socket(UDP_port);
	} else {
		print_module_label();
		LPrintln("No internet platform, could not get UDP object");
	}
}

///////////////////////////////////////////////////////////////////////////////
Loom_MaxPub::Loom_MaxPub(JsonArrayConst p)
	: Loom_MaxPub( (LoomModule::Type)(int)p[0] ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxPub::print_config()
{
	LoomPublishPlat::print_config();
	LPrintln("\tUDP Port  : ", UDP_port);
	LPrintln("\tRemote IP : ", remoteIP);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxPub::print_state()
{
	LoomPublishPlat::print_state();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxPub::set_port(uint16_t port)
{
	UDP_port = port;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_MaxPub::send_to_internet(const JsonObject json, LoomInternetPlat* plat)
{
	if (!UDP_Inst) {
		// Try to get UDP 
		UDP_Inst = (m_internet != nullptr) ? m_internet->open_socket(UDP_port) : nullptr;
		// Check if still null
		if (!UDP_Inst) {
			print_module_label();
			LPrintln("UDP_Inst is null");
			return false;
		}
	}

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Sending to remoteIP: ", remoteIP);
	}

	// Send Json	
	UDP_Inst->beginPacket(remoteIP, UDP_port);
	serializeJson(json, (*UDP_Inst) );
	UDP_Inst->endPacket(); // Mark the end of the OSC Packet
}


///////////////////////////////////////////////////////////////////////////////
void Loom_MaxPub::set_ip(IPAddress ip)
{
	remoteIP = ip;
	print_module_label();
	LPrintln("Set remote IP to: ", ip);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxPub::set_ip()
{
	print_module_label();
	LPrintln("Received command to set IP to send to");
	Loom_MaxSub* temp;
	if (device_manager && (temp = (Loom_MaxSub*)&(device_manager->MaxSub())) ) {
		IPAddress ip = temp->get_remote_IP();
		if (ip[0] != 0) {
			set_ip(ip);
		} else {
			print_module_label();
			LPrintln("IP was invalid");
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_MaxPub::dispatch(JsonObject json)
{
	JsonArray params = json["params"];
	switch( (char)json["func"] ) {
		case 's': set_ip(); return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////








