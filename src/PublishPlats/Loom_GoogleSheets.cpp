
#include "Loom_GoogleSheets.h"

///////////////////////////////////////////////////////////////////////////////
Loom_GoogleSheets::Loom_GoogleSheets(
    const char* module_name,
    const uint	internet_index,
    const char* script_url,
    const char* sheet_id,
    const char* tab_id,
    const char* device_id
)   : LoomPublishPlat( module_name, internet_index )
    , m_script_url(script_url)
    , m_sheet_id(sheet_id)
    , m_tab_id(tab_id)
    , m_device_id(device_id)
{   
    /// Build the begining of the Google Sheets URL with all of the provided parameters
    print_module_label();
    LPrintln("Google sheets ready with url: ", m_script_url);
} 

///////////////////////////////////////////////////////////////////////////////
Loom_GoogleSheets::Loom_GoogleSheets(JsonArrayConst p)
	: Loom_GoogleSheets( EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_GoogleSheets::print_config() 
{
    LoomPublishPlat::print_config();
    LPrint("\t URL: ", m_script_url, "\n");
    LPrint("\t Sheet ID: ", m_sheet_id, "\n");
    LPrint("\t Tab ID: ", m_tab_id, "\n");
    LPrint("\t Device ID: ", m_device_id, "\n");
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_GoogleSheets::send_to_internet(const JsonObject json, LoomInternetPlat* plat) 
{
    print_module_label();
    // connect to script.google.com
    Client& network = plat->connect_to_domain("script.google.com");
    // check if we connected
    if (!network.connected()) {
        LPrint("Could not connect to script.google.com");
        return false;
    }
    // start writing data to the network
    // print the initial http request
    network.print("GET ");
	// construct the URL from a bunch of different segments
    // start with the sheet metadata base, referenced from the following snprintf statement:
    /* const int printed = snprintf(m_buffer, sizeof(m_buffer), "%s?key0=sheetID&val0=%s&key1=tabID&val1=%s&key2=deviceID&val2=%s&key3=full_data&val3=", 
		script_url,                                 // URL of the script
        sheet_id,   								// Spreadsheet ID
		tab_id, 				                    // Tab to write to
		device_id);                           // The bundle source's device ID */
    network.print(m_script_url);
    network.print("?key0=sheetID&val0=");
    network.print(m_sheet_id);
    network.print("&key1=tabID&val1=");
    network.print(m_tab_id);
    network.print("&key2=deviceID&val2=");
    network.print(m_device_id);
    network.print("&key3=full_data&val3=");
    // next print the body data, converted in real time
    m_serialize_internet_impl(json, network);
    // that should finish off the URL, so print the rest of the HTTP request
	network.print(" HTTP/1.1\r\nUser-Agent: LoomOverSSLClient\r\nHost: script.google.com\r\nConnection: close\r\n\r\n");
	// all ready to go!
    if (!network.connected()) return false;
    // discard all oncoming data
    const auto start = millis();
    while (network.connected()) {
        const auto read = network.available();
        if(read) network.read(nullptr, read);
        // timeout in case connection doesn't close itself
        if (millis() - start > 5000) network.stop();
    }
    // all done!
    LPrint("Published successfully!\n");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_GoogleSheets::m_serialize_internet_impl(const JsonObject json, Print& write) 
{
    // step one: package timestamp
    const JsonObject time_obj = json["timestamp"];
    for (const JsonPair i : time_obj) {
        write.print(i.key().c_str());
        write.print('~');
        write.print(i.value().as<const char*>());
        write.print('~');
    }
    // step two: package data
    const JsonArrayConst data_ray = json["contents"];
    for (JsonArrayConst::iterator i = data_ray.begin(); i != data_ray.end();) {
        JsonObjectConst obj = i->as<JsonObjectConst>();
        // store the module name
        const char* name = obj["module"].as<const char*>();
        // iterate over the data, pushing it into the buffer
        const JsonObjectConst data_vals = obj["data"];
        // increment i, and let the loop below know if it's the very last cycle
        const bool end = (++i == data_ray.end());
        for (JsonObjectConst::iterator d = data_vals.begin();;) {
            // serialize the key
            write.print(name);
            write.print('-');
            write.print(d->key().c_str());
            write.print('~');
            // serialize the value
            const auto data_tmp = d->value();
            serializeJson(data_tmp, write);
            // add the trailing tilde, only if this is not the last element
            if (++d == data_vals.end()) {
                if (!end) write.print('~');
                break;
            }
            write.print('~');
        }
    }
}

///////////////////////////////////////////////////////////////////////////////