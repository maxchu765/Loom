#pragma once

#include "Loom_PublishPlat.h"
#include "ArduinoJson.h"


///////////////////////////////////////////////////////////////////////////////

// ### (LoomModule) | dependencies: [] | conflicts: []
/// Module taking in and translating JSON into data understood by the Google Sheets script API.
// ###
class Loom_GoogleSheets : public LoomPublishPlat 
{

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Loom Google Sheets Platform module constructor.
	/// Check out https://github.com/OPEnSLab-OSU/Loom/wiki/Using-Loom-with-Google-Sheets
	/// (or http://www.open-sensing.org/evaporometerblog/datalog) for more information on these parameters.
	/// \param[in]	module_name			String | <"Internet-Plat"> | null | Publish platform module name.
	/// \param[in]  internet_type		Set(LoomModule::Type) | <7001> | {7001("Ethernet"), 7002("WiFi")} | Code of the desired internet platform. 
	/// \param[in]  script_url			String | <""> | null | URL where the google sheets script is hosted, without domain (ex. /macros/s/AKfycby<more letters here>_k0n/exec).
	/// \param[in]  sheet_id			String | <""> | null |ID Of the spreadsheet to put data in (script must have edit access).
	/// \param[in]  tab_matches_dev_id	Bool | <true> | null | Override tab_id with the name+instance number of the device. 
	/// \param[in]  tab_id				String | <"Data"> | null | Name of the tab that the device will put data into. 
	Loom_GoogleSheets(	
						const char*			module_name,
						LoomModule::Type	internet_type,
						const char*			script_url,
						const char*			sheet_id,
						bool				tab_matches_dev_id,
						const char*			tab_id				= "Data"
					);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]  p     The array of constuctor args to expand
	Loom_GoogleSheets( JsonArrayConst p );

	/// Destructor
	~Loom_GoogleSheets() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void print_config() override;

protected:

	/// Send JSON data to a google sheet
	/// \param[in] json The JSON data, formatted according to publish();
	/// \param[in] plat A pointer to an internet platform
	/// \return True if success
	bool send_to_internet(const JsonObject json, LoomInternetPlat* plat) override;

private:

	/// Private utility send function
	bool m_serialize_internet_impl(const JsonObject json, Print& write);
	
	// use dynamic memory to store the varibles from the user
	// since most of these strings will be smaller than the maximum size

	const String m_script_url;	///< ID of Google script 
	const String m_sheet_id;	///< ID of Google Sheets spreadsheet 
	const String m_tab_id;		///< Name of tab of spreadsheet 

	const bool tab_matches_dev_id;	///< Whether or not tab of spreadsheet should be based on device name + instance

};
