///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Translator.cpp
/// @brief		File for definitions of general purpose functions to convert the 
///				type of single values or arrays
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Loom_Translator.h"
#include "Loom_Macros.h"

//=============================================================================
///@name	CONVERSION BETWEEN ARRAY FORMATS
/*@{*/ //======================================================================

///////////////////////////////////////////////////////////////////////////////
void convert_array_key_value_to_assoc(const String key_values [], String keys [], String values [], const int kv_len, const int assoc_len)
{
	if (kv_len > 2*assoc_len) {
		LPrintln("Keys or values array not large enough to hold all of keys/values, cannot split");
		return;
	}

	for (auto i = 0; i < kv_len; i+=2) {
		keys[i/2]   = key_values[i];
		values[i/2] = key_values[i+1]; 	
	}
}

///////////////////////////////////////////////////////////////////////////////
void convert_array_assoc_to_key_value(const String keys [], String values [], String key_values [], const int assoc_len, const int kv_len)
{
	if ( kv_len < 2*assoc_len ) {
		LPrintln("Key-values array is not at least twice the size of keys and values arrays, cannot merge");
		return;
	}

	for (auto i = 0; i < assoc_len; i++) {
		key_values[i*2]   = keys[i];
		key_values[i*2+1] = values[i];
	}
}

//=============================================================================
///@name	CONVERSION BETWEEN DATA TYPES
/*@{*/ //======================================================================

///////////////////////////////////////////////////////////////////////////////
int convert_string_to_int(const char* s) 
{ 
	return (int)strtol(s, NULL, 10); 
}

///////////////////////////////////////////////////////////////////////////////
float convert_string_to_float(const char* s) 
{ 
	return strtof(s, NULL); 
}

///////////////////////////////////////////////////////////////////////////////
int convert_string_to_int(const String s) 
{ 
	return (int)strtol(s.c_str(), NULL, 10); 
}

///////////////////////////////////////////////////////////////////////////////
float convert_string_to_float(const String s) 
{ 
	return strtof(s.c_str(), NULL); 
}

//=============================================================================
///@name	CONVERSION BETWEEN ARRAY TYPES 
/*@{*/ //======================================================================

///////////////////////////////////////////////////////////////////////////////
void convert_array(const String src [], int dest [], const int count)
{ 
	char buf[20]; 
	for (int i = 0; i < count; i++) { 
		src[i].toCharArray(buf, 20); 
		dest[i] = (int)strtol(buf, NULL, 10); 
	} 
}

///////////////////////////////////////////////////////////////////////////////
void convert_array(const String src [], float dest [], const int count)
{ 
	char buf[20]; 
	for (int i = 0; i < count; i++) { 
		src[i].toCharArray(buf, 20); 
		dest[i] = strtof(buf, NULL); 
	} 
}

///////////////////////////////////////////////////////////////////////////////
void convert_array(const String src [], char dest [][20], const int count)
{ 
	for (int i = 0; i < count; i++) 
		src[i].toCharArray(dest[i], 10); 
}

///////////////////////////////////////////////////////////////////////////////



