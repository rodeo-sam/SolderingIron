#pragma once

// #define HW_VERSION 1
#define HW_VERSION 12 // current hardware version

// the temperature the system is supposed to not exceed
#define TEMP_MAX 380

// temperature that should be held when resting
#define TEMP_REST 150

// lowest temperature allowed
#define TEMP_MIN 0 

// default power on temperature
#define DEFAULT_TEMP 300
#define SAVED_TEMP0 350
#define SAVED_TEMP1 250

//PID-Controller values
#define PID_P 350
#define PID_I 2
#define PID_D 10

// threshold at which the hall sensor is supposed to trigger
// if set to 0 rest detection will be disabled
#define REST_HALL_THRESHOLD 15

// temperature scale
// if defined the display will work in fahrenheit
//#define TEMP_KELVIN
#define TEMP_CELSIUS
//#define TEMP_FAHRENHEIT
//#define TEMP_RANKINE
//#define TEMP_DELISLE
//#define TEMP_REAMUR
//#define TEMP_NEWTON
//#define TEMP_ROMER
