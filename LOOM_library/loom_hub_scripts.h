// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <math.h>


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
// Used to make function signatures easier to read 
// when returning function pointers
typedef float (*retFuncPtr)(float,float); 

#define max_script_len 20
// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 

// Used to save 
struct config_dynamic_scripts_t {
	int num_dynamic_scripts;
	char* dynamic_scripts[5][max_script_len]; // currently up to 5 scripts with as many as 20 commands (arbitrary numbers right now)
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

struct config_dynamic_scripts_t config_dynamic_scripts;

// Scripts that are preloaded and are saved in program memory not flash
int num_static_scripts;
// char* static_scripts[10][20];
// String static_scripts[10][20];

OSCMessage* static_msg_scripts[10];

// Used by parser
float stack[50];    
int   stackPtr = 0;
char  data_type;
char  buf[20];
bool  takingElseBranch = false;

// Data registers that persist outside of parser
float reg[3][10];

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================

void run_all_scripts();
void run_all_static_scripts();
void run_all_dynamic_scripts();
int  get_script_len(String script[]);

retFuncPtr strToFunc(char * str);
float msgGetLiteral(OSCMessage* msg, int pos);
void  parseScript(OSCMessage* msg);
void  parseScript(OSCBundle* bndl);



// ================================================================ 
// ===                      MINI FUNCTIONS                      === 
// ================================================================

// Returns the value in the ith register of the R (0), S (1), or T (2) bank
float load_reg(int r, int i) { return reg[r][i]; }
// Sets the value in the ith register of the R (0), S (1), or T (2) bank to be val	
void  store_reg(int r, int i, float val) { reg[r][i] = val; }


// Single value math
float inc(float x, float n) { return x+1; } 
float dec(float x, float n) { return x-1; }
float square(float x, float n) { return x * x; }

// Math
float addition(float x, float y) { return x + y; }
float subtract(float x, float y) { return x - y; }
float multiply(float x, float y) { return x * y; }
float divide(  float x, float y) { return x / y; }
float exp(   float x, float y) { return pow(x, y); }

// Comparisons
float equal(        float x, float y) { return x == y; }
float notEqual(     float x, float y) { return x != y; }
float lessThan(     float x, float y) { return x <  y; }
float greaterThan(  float x, float y) { return x >  y; }
float lessThanEq(   float x, float y) { return x <= y; }
float greaterThanEq(float x, float y) { return x >= y; }

// Logic gates
float logical_not( float a, float n) { return !(bool)a; } 
float logical_or(  float a, float b) { return (bool)a || (bool)b; } 
float logical_and( float a, float b) { return (bool)a && (bool)b; } 
float logical_nor( float a, float b) { return !((bool)a || (bool)b); } 
float logical_nand(float a, float b) { return !((bool)a && (bool)b); }
float logical_xor( float a, float b) { return ( ((bool)a && !(bool)b) || (!(bool)a && (bool)b) ); } 

// Printing / Void fuctions
float ifFuncEval(float y, float z) { Serial.println("Evaluated True");  return 0; }
float elFuncEval(float y, float z) { Serial.println("Evaluated False"); return 0; }
float termFunc(  float y, float z) { Serial.println("Function chain terminated"); return 0; }
float errorFunc( float y, float z) { Serial.println("Function pointer was null"); return 0; }
float printVal(  float y, float z) { Serial.print("Value: "); Serial.println(y);  return 0; }

// Wrapper for any 'float(*ptr)(float,float)' functions
float wrapper(float x, float y, float (*fPtr)(float,float)) {
	return fPtr(x, y);
}


// ================================================================
// ===                          SETUP                           ===
// ================================================================


void setup_hub_scripts()
{
	LOOM_DEBUG_Println("Setting up hub scripts");

	// Clear out 'registers'
	for (int b = 0; b < 3; b++) {
		for (int r = 0; r < 10; r++) {
			reg[b][r] = 0.0;
		}
	}

	// Setup static scripts 
	// i.e. num_static_scripts
	// static_scripts[0][0] = "8"; //"150", "blink_ex", "done"};
	// static_scripts[0][1] = "150";
	// static_scripts[0][2] = "blink_ex";
	// static_scripts[0][3] = "done";


	static_msg_scripts[0] = new OSCMessage("/test");
	static_msg_scripts[0]->add(10.0).add(300.0).add("blink_ex");

	static_msg_scripts[1] = new OSCMessage("/test2");
	static_msg_scripts[1]->add(4.0).add(1000.0).add("blink_ex");

	num_static_scripts = 2;

	// Setup dynamic scripts
	// i.e. read in from flash
}


// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================

// Runs all scripts
void run_all_scripts()
{
	run_all_static_scripts();
	run_all_dynamic_scripts();
}

void run_all_static_scripts()
{
	LOOM_DEBUG_Println("In run_all_static_scripts");
	OSCBundle scriptBndl;
	for (int i = 0; i < num_static_scripts; i++) {

	// BUNDLE ARRAY VERSION
		// print_bundle(&static_msg_scripts[i]);
		parseScript(static_msg_scripts[i]);

	// STRING ARRAY VERSION

		// // ‘Not actually a key-value array, but this allows the interpret functionality to be used (essential)
		// convert_key_value_array_to_bundle(static_scripts[i], &scriptBndl, "/addr", get_script_len(static_scripts[i]), SINGLEMSG, 4);
	
		// // print_bundle(&scriptBndl);

		// parseScript(&scriptBndl);
	}
}

void run_all_dynamic_scripts()
{
	// for (int i = 0; i < config_dynamic_scripts.num_dynamic_scripts; i++) {
		
	// }
}



int get_script_len(String script[])
{
	for (int i = 0; i < max_script_len; i++) {
		LOOM_DEBUG_Println2("\t", script[i]);
		if (script[i] == "done")  
			return i;
	}

	return max_script_len;
}



// ================================================================
// ===                      SCRIPT PARSER                       ===
// ================================================================


void parseScript(OSCMessage* msg)
{
	// print_message(msg);

	char buf[50];
	msg->getAddress(buf, 0);
	LOOM_DEBUG_Println2("Running Script: ", buf );
	for (int i = 0; i < msg->size(); i++) {

		data_type = msg->getType(i);
		Serial.print("Data Type: "); Serial.print(data_type); Serial.print(" : ");

		switch(data_type) {
			// Number or boolean literals
			case 'd': case 'f': case 'i': case 'b':
				Serial.println(     msgGetLiteral(msg, i));
				stack[stackPtr++] = msgGetLiteral(msg, i);
				break;

			// Commands and Functions and If Statements
			case 's': {
				msg->getString(i, buf, 20);
				Serial.println(buf);

				// Pop one value
				if ( strcmp(buf, "pop") == 0 ) {  
					stackPtr--;  break;  
				}

				// Duplicate the value on the top of the stack
				if ( strcmp(buf, "dup") == 0 ) {  
					stack[stackPtr] = stack[stackPtr-1]; stackPtr++;  break;  
				}


				// Unary Math operators
				if ( (strcmp(buf, "++") == 0) || (strcmp(buf, "--") == 0) || (strcmp(buf, "sqr") == 0) ) {  
					stack[stackPtr-1] = strToFunc(buf)(stack[stackPtr-1], 0.0);  break;  
				}

				// Math operators
				if ( (strcmp(buf,"+")==0) || (strcmp(buf,"-")==0) || (strcmp(buf,"*")==0) || (strcmp(buf,"/")==0) || (strcmp(buf,"^")==0)) {
					stack[stackPtr-2] = strToFunc(buf)(stack[stackPtr-2], stack[stackPtr-1]);  stackPtr--;  break;
				}

				// Reading from External Variables 
				if (strncmp(buf, "load_", 5) == 0) {  
					stack[stackPtr++] = reg[ buf[5]-114 ][ (int)strtol(buf+6,NULL,10) ];  break;  
				}
				
				// Storing to External Variables
				if (strncmp(buf, "store_", 6) == 0) {  
					reg[ buf[6]-114 ][ (int)strtol(buf+7,NULL,10) ] = stack[stackPtr-1];  break;  
				}

				// If Statements
				if ( strcmp(buf, "if") == 0 ) {      
					// Grab the condition value from the stack
					// If taking the 'else' branch, skip the 'then' branch
					if (!stack[--stackPtr]) {
						Serial.println("Taking Else branch");
						takingElseBranch = true;
						msg->getString(++i, buf, 20);
						while ( (strcmp(buf, "else") != 0) && (strcmp(buf, "endif") != 0) ) {
							msg->getString(++i, buf, 20);
						}
					} else {
						Serial.println("Taking Then branch");
					}
					break;
				}

				if ( strcmp(buf, "else") == 0 ) {  
					// If taking the else branch, just continue
    				// Otherwise skip to 'endif'
					if (!takingElseBranch) {
						msg->getString(++i, buf, 20);
						while( strcmp(buf, "endif") != 0 ) {
							msg->getString(++i, buf, 20);
						}
					}
					takingElseBranch = false;
					break;
				}

				if ( strcmp(buf, "endif") == 0) {
					// probably don't need to do anything here
					// takingElseBranch = false;
					break;
				}

				if ( strcmp(buf, "printStack") == 0) {
					#if LOOM_DEBUG == 1
						Serial.println();
						for (int j = 0; j < stackPtr; j++) {
							LOOM_DEBUG_Print2("  ", stack[j]); 
						}
						Serial.println();
					#endif
					break;
				}



				// If none of the above commands matched, try seaching external functions
				float (*fPtr)(float,float) = strToFunc(buf);
				if ( fPtr ) {
					stack[stackPtr-2] = fPtr(stack[stackPtr-2], stack[stackPtr-1]);  stackPtr--;  
					break;
				}


				// No 'break' so if error, fallthrough to 'default' case
			} // of case 's':

			default:
				LOOM_DEBUG_Println2("Parser error, invalid program detected at symbol #: ", i);
				break;
		}


// This is just for debugging purposes for now
		Serial.print("Stack ("); Serial.print(i); Serial.print("):");
		for (int j = 0; j < stackPtr; j++) {
			Serial.print("  "); Serial.print(stack[j]);
		} 
		Serial.println();

	}

	Serial.print("Result: ");
	Serial.println(stack[stackPtr-1]);
	Serial.println();

	// These just print out the register contents for debugging purposes
	// Serial.print("R: ");
	// for (int i = 0; i < 10; i++) {
	// 	Serial.print(i); Serial.print(": "); Serial.print(reg[0][i]); Serial.print(", "); 
	// }
	// Serial.print("S: ");
	// for (int i = 0; i < 10; i++) {
	// 	Serial.print(i); Serial.print(": "); Serial.print(reg[1][i]); Serial.print(", "); 
	// }
	// Serial.print("T: ");
	// for (int i = 0; i < 10; i++) {
	// 	Serial.print(i); Serial.print(": "); Serial.print(reg[2][i]); Serial.print(", "); 
	// }
}

void parseScript(OSCBundle* bndl)
{
	if (bndl->size() != 1) {
		LOOM_DEBUG_Println("Bundle not in the correct format for script parser");
		LOOM_DEBUG_Println("Call with either message or bundle containing single message");
	}
	parseScript(bndl->getOSCMessage(0));
}



// --- MESSAGE GET LITERAL ---
// 
// This function attempts to grab a number or boolean
// value from a message and return it as a float.
// It works if the value is a double, float, int, or bool
//
float msgGetLiteral(OSCMessage* msg, int pos) 
{
	switch(msg->getType(pos)) {
		case 'd': return (float)msg->getDouble(pos);
		case 'f': return (float)msg->getFloat(pos); 
		case 'i': return (float)msg->getInt(pos); 			
		case 'b': return (float)msg->getBoolean(pos);
		default:  return (float)-1.0;
	}
}



// --- STRING TO FUNCTION ---
//
// This function takes a string that matches or
// represents a function, and returns a pointer of 
// the corresponding function
//
retFuncPtr strToFunc(char * str) {
	// Math Unary
	if ( (strcmp(str, "inc") == 0)    || (strcmp(str, "++")  == 0) ) return inc;
	if ( (strcmp(str, "dec") == 0)    || (strcmp(str, "--")  == 0) ) return dec;
	if ( (strcmp(str, "square") == 0) || (strcmp(str, "sqr") == 0) ) return square;
	
	// Math 
	if ( (strcmp(str, "add") == 0)           || (strcmp(str, "+") == 0 ) ) return addition;
	if ( (strcmp(str, "subtract") == 0)      || (strcmp(str, "-") == 0 ) ) return subtract;
	if ( (strcmp(str, "multiply") == 0)      || (strcmp(str, "*") == 0 ) ) return multiply;
	if ( (strcmp(str, "divide") == 0)        || (strcmp(str, "/") == 0 ) ) return divide;
	if ( (strcmp(str, "exp") == 0)           || (strcmp(str, "^") == 0 ) ) return exp;	

	// Comparison
	if ( (strcmp(str, "equal") == 0)         || (strcmp(str, "==") == 0) ) return equal;
	if ( (strcmp(str, "notEqual") == 0)      || (strcmp(str, "!=") == 0) ) return notEqual;
	if ( (strcmp(str, "lessThan") == 0)      || (strcmp(str, "<")  == 0) ) return lessThan;
	if ( (strcmp(str, "greaterThan") == 0)   || (strcmp(str, ">")  == 0) ) return greaterThan;
	if ( (strcmp(str, "lessThanEq") == 0)    || (strcmp(str, "<=") == 0) ) return lessThanEq;
	if ( (strcmp(str, "greaterThanEq") == 0) || (strcmp(str, ">=") == 0) ) return greaterThanEq;
	
	// Logic
	if ( (strcmp(str, "not")  == 0)  || (strcmp(str, "!")  == 0) ) return logical_not;
	if ( (strcmp(str, "or")   == 0)  || (strcmp(str, "||") == 0) ) return logical_or;
	if ( (strcmp(str, "and")  == 0)  || (strcmp(str, "&&") == 0) ) return logical_and;
	if ( (strcmp(str, "nor")  == 0)  || (strcmp(str, "!|") == 0) ) return logical_nor;
	if ( (strcmp(str, "nand") == 0)  || (strcmp(str, "!&") == 0) ) return logical_nand;
	if ( (strcmp(str, "xor")  == 0)  || (strcmp(str, "x|") == 0) ) return logical_xor;
	
	// Other
	if ( (strcmp(str, "ifFuncEval") == 0) ) return ifFuncEval;
	if ( (strcmp(str, "elFuncEval") == 0) ) return elFuncEval;
	if ( (strcmp(str, "termFunc")   == 0) ) return termFunc;
	if ( (strcmp(str, "errorFunc")  == 0) ) return errorFunc;
	if ( (strcmp(str, "printVal")   == 0) ) return printVal;


	// Custom functions get called here
	return custom_strToFunc(str);

	// return NULL;
}



