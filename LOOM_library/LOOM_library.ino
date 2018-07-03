// ================================================================
// ===              INCLUDE CONFIGURATION FILE                  ===
// ===    INCLUDE DECLARATIONS, STRUCTS, AND FUNCTIONS FROM     ===
// ===            OTHER FILES AS SET IN CONFIG.H                ===
// ================================================================

// Config has to be first has it hold all user specified options
#include "config.h"

// Preamble includes any relevant subroutine files based on options
// specified in the above config
#include "loom_preamble.h"

// ================================================================ 
// ===                           SETUP                          ===
// ================================================================ 
void setup() 
{
	// LOOM_begin calls any relevant (based on config) LOOM device setup functions
	Loom_begin();	

	// Any custom setup code
}

// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() 
{
	OSCBundle bndl, send_bndl; 

	// Receive bundles, takes bundle to be filled and wireless platforms [WIFI, LORA, NRF]
	// receive_bundle(&bndl, WIFI);

	// Process bundle (nothing will happen if bndl is empty), bundle is emptied after processing
	// process_bundle(&bndl);

// 	// Dummy Sapflow Data
	String parameters[7] = {"Test", "Battery", "Temp1", "Temp2", "Temp_Diff", "Temp_SHT31D", "Humidity_SHT31D"};
	String data[7] = {"abc", "4.27", "23.0", "25.2", "2.2", "30.0", "14"};


// 	convert_assoc_arrays_to_bundle(parameters, data, &bndl, "/some/packet/header", 7, SINGLEMSG);

// 	print_bundle(&bndl);

	sd_save_array("time0.txt", data, 7, ',', 0);
	read_all_from_file("time0.txt");

	sd_save_array("time1.txt", data, 7, ',', 1);
	read_all_from_file("time1.txt");

	sd_save_array("time2.txt", data, 7, ',', 2);
	read_all_from_file("time2.txt");

	sd_save_array("time3.txt", data, 7, ',', 3);
	read_all_from_file("time3.txt");

	sd_save_array("time4.txt", data, 7, ',', 4);
	read_all_from_file("time4.txt");

	while(1);

	// sd_save_bundle("example.txt", &bndl, 1);

// 	read_all_from_file("example.txt");

// 	sd_empty_file("example.txt");
	// read_all_from_file("example8.txt");

	// Node
	send_bundle(&send_bndl, SDCARD);
	send_bundle(&send_bndl, LORA);


	// HUB
	receive_bundle(&bndl);
	send_bundle(&bndl, SDCARD);
// 	while(1);

	// Update stored readings from sensors
	// measure_sensors();

	// Populate bundle to send with sensor values
	// package_data(&send_bndl);

	// Send the bundle, takes bundle to be filled and platform to send it over [WIFI, LORA, NRF, SDCARD, PUSHINGBOX]
	// send_bundle(&bndl, SD);

	// while(1);


	delay(2000);

	// Loop checks and sleep between iterations if enabled
	additional_loop_checks();
	
} // End loop section








